/****************************************************************************
  FileName     [ vlpEXParse.cpp ]
  PackageName  [ vlp ]
  Synopsis     [ Complementary Extra File Extension Parser. ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_EXPARSE_SOURCE
#define VLP_EXPARSE_SOURCE

#include "vlpDesign.h"
#include "cktModule.h"
#include "util.h"
#include <vector>
#include <set>

extern CktModule* curCktModule;

// Global Member
static char buffer[buffer_size];
static string token_str;
static vector<string> tokens;
static int tempId;

/********** Berkeley Logic Interchange Format (BLIF) **********/
bool
VlpDesign::lexLineBLIF(FILE* input) {
   unsigned i;
   tokens.clear();
   token_str = "";
   
   while (!feof(input)) {
      fgets(buffer, buffer_size, input); if (feof(input)) break;
      for (i = 0; i < strlen(buffer); ++i) {
         if ((buffer[i] == ' ') || (buffer[i] == '\t')) {
            if (token_str.size()) tokens.push_back(token_str);
            token_str = "";
         }
         else if ((buffer[i] == '\n') || (buffer[i] == '#')) {  // Comment Symbol for BLIF
            if (token_str.size()) tokens.push_back(token_str);
            token_str = ""; break;
         }
         else if (buffer[i] == '\\') break;  // Concatenation Symbol for BLIF
         //else if (buffer[i] == '[') token_str += '(';
         //else if (buffer[i] == ']') token_str += ')';
         else token_str += buffer[i];
      }
      if (!((i < strlen(buffer)) && (buffer[i] == '\\'))) {
         if (token_str.size()) tokens.push_back(token_str);
         if (tokens.size()) return true;
      }
   }
   return false;
}

void
VlpDesign::creGateBLIF(vector<string>& inputs, string& output, set<string>& covers, bool onset) {
   set<string> gate_template;
   string minterm = "";
   int input_size = inputs.size();
   int output_size = 1;
   
   // Match Primitive Gates
   if (inputs.size() == 0) {  // const 0/1
      curCktModule->creConstCell(1, output, "'b" + myInt2Str(onset ? 1 : 0)); return;
   }
   else if (inputs.size() == 1) {
      // buf (inv)
      gate_template.clear(); gate_template.insert("1");
      if (covers == gate_template) {
         if (onset) curCktModule->creCell(CKT_BUF_CELL, input_size, output_size, output);
         else curCktModule->creCell(CKT_INV_CELL, input_size, output_size, output);
         curCktModule->crePreOut(output, inputs); return;
      }
      // inv (buf)
      gate_template.clear(); gate_template.insert("0");
      if (covers == gate_template) {
         if (onset) curCktModule->creCell(CKT_INV_CELL, input_size, output_size, output);
         else curCktModule->creCell(CKT_BUF_CELL, input_size, output_size, output);
         curCktModule->crePreOut(output, inputs); return;
      }
   }
   else {  // two more inputs logic gates
      // and (namd)
      gate_template.clear(); minterm = "";
      for (unsigned i = 0; i < inputs.size(); ++i) minterm += "1";
      gate_template.insert(minterm);
      if (covers == gate_template) {
         if (onset) curCktModule->creCell(CKT_AND_CELL, input_size, output_size, output);
         else curCktModule->creCell(CKT_NAND_CELL, input_size, output_size, output);
         curCktModule->crePreOut(output, inputs); return;
      }
      // nor (or)
      gate_template.clear(); minterm = "";
      for (unsigned i = 0; i < inputs.size(); ++i) minterm += "0";
      gate_template.insert(minterm);
      if (covers == gate_template) {
         if (onset) curCktModule->creCell(CKT_NOR_CELL, input_size, output_size, output);
         else curCktModule->creCell(CKT_OR_CELL, input_size, output_size, output);
         curCktModule->crePreOut(output, inputs); return;
      }
      // nand (and)
      gate_template.clear();
      for (unsigned i = 0; i < inputs.size(); ++i) {
         minterm = ""; for (unsigned j = 0; j < inputs.size(); ++j) minterm += "-"; minterm[i] = '0';
         gate_template.insert(minterm);
      }
      if (covers == gate_template) {
         if (onset) curCktModule->creCell(CKT_NAND_CELL, input_size, output_size, output);
         else curCktModule->creCell(CKT_AND_CELL, input_size, output_size, output);
         curCktModule->crePreOut(output, inputs); return;
      }
      // or (nor)
      gate_template.clear();
      for (unsigned i = 0; i < inputs.size(); ++i) {
         minterm = ""; for (unsigned j = 0; j < inputs.size(); ++j) minterm += "-"; minterm[i] = '1';
         gate_template.insert(minterm);
      }
      //for (set<string>::const_iterator it = covers.begin(), is = gate_template.begin(); it != covers.end() && is != gate_template.end(); ++it, ++is)
      //   cout << "Cover = " << (*it) << ", Template = " << (*is) << endl;
      if (covers == gate_template) {
         if (onset) curCktModule->creCell(CKT_OR_CELL, input_size, output_size, output);
         else curCktModule->creCell(CKT_NOR_CELL, input_size, output_size, output);
         curCktModule->crePreOut(output, inputs); return;
      }
      // xor (xnor)
      // Arbitrary SOP Format : Brute-force build PLA
      curCktModule->SOPGate(inputs, output, covers, onset); return;
   }
}

void
VlpDesign::parseBLIF(FILE* file) {
   // Variables Declaration
   vector<string> inputs;
   set<string> covers;
   string modName, gateName;
   bool start, input, output, onset;
   // Initialization
   inputs.clear(); covers.clear();
   modName = gateName = "";
   start = input = output = onset = false;
   // Parse
   while (lexLineBLIF(file)) {
      assert (tokens.size());
      if (tokens[0] == ".model") {  // .model modName
         if (modName != "") {
            Msg(MSG_ERR) << "Hierarchical design not supported in Blif-format." << endl;
            curCktModule = 0; return;
         }
         if (tokens.size() == 1) {
            Msg(MSG_ERR) << "NO module name assigned after .model." << endl;
            curCktModule = 0; return;
         }
         else if (tokens.size() > 2) {
            Msg(MSG_ERR) << "More than one module name assigned after .model." << endl;
            curCktModule = 0; return;
         }
         // Create top module
         modName = tokens[1];
         curCktModule = CreateDesign(modName); assert (curCktModule);
         
         // Chengyin Debug
         //cout << "Design = " << modName << endl;
      }
      else if (tokens[0] == ".inputs") {  // .inputs in1 in2 ...
         if (modName == "") {
            Msg(MSG_ERR) << "Missing .model specification." << endl;
            curCktModule = 0; return;
         }
         if (input) {
            Msg(MSG_ERR) << "Input ports are already assigned." << endl;
            curCktModule = 0; return;
         }
         if (tokens.size() == 1) {
            Msg(MSG_ERR) << "NO input name assigned after .inputs." << endl;
            curCktModule = 0; return;
         }
         // Create PI Gates
         input = true; inputs.clear();
         for (unsigned i = 1; i < tokens.size(); ++i) inputs.push_back(tokens[i]);
         if (!(curCktModule->creIO(1, inputs, CKT_PI_CELL))) {
            Msg(MSG_ERR) << "Create PI Gate Failed!" << endl;
            curCktModule = 0; return;
         }
         
         // Chengyin Debug
         //cout << "Inputs = "; for (unsigned i = 0; i < inputs.size(); ++i) cout << inputs[i] << " "; cout << endl;
      }
      else if (tokens[0] == ".outputs") {
         if (modName == "") {
            Msg(MSG_ERR) << "Missing .model specification." << endl;
            curCktModule = 0; return;
         }
         if (output) {
            Msg(MSG_ERR) << "Output ports are already assigned." << endl;
            curCktModule = 0; return;
         }
         if (tokens.size() == 1) {
            Msg(MSG_ERR) << "NO output name assigned after .inputs." << endl;
            curCktModule = 0; return;
         }
         // Create PO Gates
         output = true; inputs.clear();
         for (unsigned i = 1; i < tokens.size(); ++i) inputs.push_back(tokens[i]);
         if (!(curCktModule->creIO(1, inputs, CKT_PO_CELL))) {
            Msg(MSG_ERR) << "Create PO Gate Failed!" << endl;
            curCktModule = 0; return;
         }
         
         // Chengyin Debug
         //cout << "Outputs = "; for (unsigned i = 0; i < inputs.size(); ++i) cout << inputs[i] << " "; cout << endl;
      }
      else if (tokens[0] == ".latch") {  // .latch <input> <output> [<type> <control>] [<init-val>]
         if (modName == "") {
            Msg(MSG_ERR) << "Missing .model specification." << endl;
            curCktModule = 0; return;
         }
         if (gateName != "") creGateBLIF(inputs, gateName, covers, onset);  // Record Gate
         if (tokens.size() < 5) {
            Msg(MSG_ERR) << "Incomplete latch declaration after .latch." << endl;
            curCktModule = 0; return;
         }
         // Create DFF : Note that we omit type, control and always set init-val = 0
         inputs.clear(); inputs.push_back(tokens[1]); gateName = "";
         curCktModule->creDffCell(1, tokens[2], tokens[4]);
         curCktModule->crePreOut(tokens[2], inputs);
      }
      else if (tokens[0] == ".names") {  // .names [in1 in2 ...] out
         if (modName == "") {
            Msg(MSG_ERR) << "Missing .model specification." << endl;
            curCktModule = 0; return;
         }
         if (gateName != "") creGateBLIF(inputs, gateName, covers, onset);  // Record Gate
         if (tokens.size() == 1) {
            Msg(MSG_ERR) << "NO gate name assigned after .names." << endl;
            curCktModule = 0; return;
         }
         // Create Logic Gates
         gateName = tokens.back(); tokens.pop_back(); inputs.clear(); covers.clear(); onset = false;
         for (unsigned i = 1; i < tokens.size(); ++i) inputs.push_back(tokens[i]);
         
         // Chengyin Debug
         //cout << "Names = ";
         //for (unsigned i = 0; i < inputs.size(); ++i) cout << inputs[i] << " ";
         //cout << ", " << gateName << endl;
      }
      else if (tokens[0] == ".end") {  // .end
         if (modName == "") {
            Msg(MSG_ERR) << "Missing .model specification." << endl;
            curCktModule = 0; return;
         }
         if (gateName != "") creGateBLIF(inputs, gateName, covers, onset);  // Record Gate
         // End Module
         if (curCktModule->checkConn(modName) != 0) {
            Msg(MSG_ERR) << "Check module connection failed." << endl;
            curCktModule = 0;
         }
         return;
      }
      else {  // 001-11 1 ...
         if (gateName == "") {
            Msg(MSG_ERR) << "Unknown statement \"" << tokens[0] << "\" !" << endl;
            curCktModule = 0; return;
         }
         if (inputs.size()) {
            if (tokens.size() != 2) {
               Msg(MSG_ERR) << "Illegal SOP format for gate \"" << gateName << "\" !" << endl;
               curCktModule = 0; return;
            }
            //cout << "Token0 = \"" << tokens[0] << "\" Token1 = \"" << tokens[1] << "\"" << endl;
            if (tokens[1].size() != 1) {
               Msg(MSG_ERR) << "Output for gate \"" << gateName << "\" has more than one char !" << endl;
               curCktModule = 0; return;
            }
            // check onset / offset
            if ((tokens[1] != "1") && (tokens[1] != "0")) {
               Msg(MSG_ERR) << "Output for gate \"" << gateName << "\" is not 0/1 !" << endl;
               curCktModule = 0; return;
            }
            if (covers.size()) {
               if (onset ^ (tokens[1] == "1")) {
                  Msg(MSG_ERR) << "Gate \"" << gateName << "\" : Cannot specify both onset and offset !" << endl;
                  curCktModule = 0; return;
               }
            }
            else onset = (tokens[1] == "1");
            // check SOP
            if (tokens[0].size() != inputs.size()) {
               Msg(MSG_ERR) << "Mismatch input SOP for gate \"" << gateName << "\" !" << endl;
               curCktModule = 0; return;
            }
            for (unsigned i = 0; i < tokens[0].size(); ++i) {
               if ((tokens[0][i] != '1') && (tokens[0][i] != '0') && (tokens[0][i] != '-')) {
                  Msg(MSG_ERR) << "Unknown SOP specification symbol (" << tokens[0][i] << ") for gate \"" << gateName << "\" !" << endl;
                  curCktModule = 0; return;
               }
            }
            covers.insert(tokens[0]);
         }
         else {  // const
            if (tokens.size() > 1) {
               Msg(MSG_ERR) << "Illegal SOP format for const gate \"" << gateName << "\" !" << endl;
               curCktModule = 0; return;
            }
            if (tokens[0].size() != 1) {
               Msg(MSG_ERR) << "Output for const gate \"" << gateName << "\" has more than one char !" << endl;
               curCktModule = 0; return;
            }
            // check onset / offset
            if ((tokens[0] != "1") && (tokens[0] != "0")) {
               Msg(MSG_ERR) << "Output for const gate \"" << gateName << "\" is not 0/1 !" << endl;
               curCktModule = 0; return;
            }
            if (covers.size()) {
               Msg(MSG_ERR) << "Const Gate \"" << gateName << "\" : Repeated specification !" << endl;
               curCktModule = 0; return;
            }
            else onset = (tokens[0] == "1");
         }
      }
   }
   // File end reached without .end
   Msg(MSG_ERR) << "Are you missing .end ?!" << endl;
   curCktModule = 0;
}

/********** Bit-Precise Modelling of Word-Level Problems for Model Checking (BTOR) **********/
bool
VlpDesign::lexLineBTOR(FILE* input) {
   unsigned i;
   tokens.clear();
   token_str = "";
   
   while (!feof(input)) {
      fgets(buffer, buffer_size, input); if (feof(input)) break;
      for (i = 0; i < strlen(buffer); ++i) {
         if ((buffer[i] == ' ') || (buffer[i] == '\t')) {
            if (token_str.size()) tokens.push_back(token_str);
            token_str = "";
         }
         else if ((buffer[i] == '\n') || (buffer[i] == ';')) {  // Comment Symbol for BTOR
            if (token_str.size()) tokens.push_back(token_str);
            token_str = ""; break;
         }
         else token_str += buffer[i];
      }
      if (token_str.size()) tokens.push_back(token_str);
      if (tokens.size()) return true;
   }
   return false;
}

inline string nameFromId(const int& id) { assert (id > 0); return "n" + myInt2Str(id); }
inline string tempFromId(const int& id) { assert (id > 0); return "temp" + myInt2Str(id); }

string
VlpDesign::creGateBTOR(const int& op_in) {
   if (op_in < 0) {
      int width = curCktModule->getOutPinWidth(nameFromId(-op_in)); assert (width);
      creGateBTOR(CKT_INV_CELL, width, tempFromId(++tempId), nameFromId(-op_in));
      return tempFromId(tempId);
   }
   else return nameFromId(op_in);
}

void
VlpDesign::creGateBTOR(const int& width, const string& name, const string& value_str) {
   curCktModule->creConstCell(width, name, value_str);
}

void
VlpDesign::creGateBTOR(const CktCellType& cellType, const int& width, const string& name, const string& inName1, const string& inName2) {
   int var_width = width;
   if (inName1 == "") {
      assert ((cellType == CKT_PI_CELL) || (cellType == CKT_PO_CELL) || (cellType == CKT_PIO_CELL));
      vector<string> outName; outName.clear(); outName.push_back(name);
      curCktModule->creIO(width, outName, cellType);
   }
   else {
      vector<string> inName; inName.clear();
      inName.push_back(inName1);
      if (inName2 != "") inName.push_back(inName2);
      curCktModule->creCell(cellType, inName.size(), var_width, name);
      curCktModule->crePreOut(name, inName);
   }
}

void
VlpDesign::parseBTOR(FILE* file) {
   string op, opStr1, opStr2, opStr3;
   int id, width, op1, op2, op3;

   // Initialize temporary gate id
   tempId = 0;
   
   // Create top module
   curCktModule = CreateDesign("BTOR_Design"); assert (curCktModule);

   while (lexLineBTOR(file)) {
      assert (tokens.size());
      // id
      if (!myStr2Int(tokens[0], id)) {
         Msg(MSG_ERR) << "Line start with non-integer id \"" << tokens[0] << "\" !" << endl;
         curCktModule = 0; return;
      }
      // op
      if (tokens.size() == 1) {
         Msg(MSG_ERR) << "Missing operator type for variable " << id << " !" << endl;
         curCktModule = 0; return;
      }
      op = tokens[1];
      // width
      if (tokens.size() == 2) {
         Msg(MSG_ERR) << "Missing width specification for variable " << id << " !" << endl;
         curCktModule = 0; return;
      }
      if ((!myStr2Int(tokens[2], width)) || (!(width > 0))) {
         Msg(MSG_ERR) << "Variable " << id << " without positive integer width specification \"" << tokens[2] << "\" !" << endl;
         curCktModule = 0; return;
      }
      if (op == "var") { // new variable : id var width
         creGateBTOR(CKT_PI_CELL, width, nameFromId(id));
      }
      else if ( // 1 operand operators
            op == "constd" || op == "consth" || op == "const" || op == "root" || op == "one" || 
            op == "ones" || op == "zero" || op == "not" || op == "neg" || op == "inc" || 
            op == "dec" || op == "redand" || op == "redor" || op == "redxor" 
            )
      {
         if (tokens.size() < 4) {
            Msg(MSG_ERR) << "Missing operand 1 for variable " << id << " (" << op << ") !" << endl;
            curCktModule = 0; return;
         }
         if (op == "constd") { creGateBTOR(width, nameFromId(id), "'d" + tokens[3]); continue; }
         else if (op == "consth") { creGateBTOR(width, nameFromId(id), "'h" + tokens[3]); continue; }
         if (op == "const") { creGateBTOR(width, nameFromId(id), "'b" + tokens[3]); continue; }
         else if (!myStr2Int(tokens[3], op1)) {
            Msg(MSG_ERR) << "Variable " << id << " has non-integer operand \"" << tokens[3] << "\" !" << endl;
            curCktModule = 0; return;
         }
         if (op == "root") { // assertion : id root width op1
            if (width != 1) {
               Msg(MSG_ERR) << "Bit-width of root should be 1 !" << endl;
               curCktModule = 0; return;
            }
            if (op1 < 0) creGateBTOR(CKT_INV_CELL, width, nameFromId(id), nameFromId(-op1));
            else creGateBTOR(CKT_BUF_CELL, width, nameFromId(id), nameFromId(op1));
            creGateBTOR(CKT_PO_CELL, width, nameFromId(id));
         }
         else if (op == "one") // constant 1 : id op width
            creGateBTOR(width, nameFromId(id), "'b" + myInt2Str(1));
         else if (op == "ones") // constant -1 : id op width
            creGateBTOR(width, nameFromId(id), "'b" + string(width, '1'));
         else if (op == "zero") // constant 0 : id op width
            creGateBTOR(width, nameFromId(id), "'b" + myInt2Str(0));
         else if (op == "not") {  // inverter : id not width op1
            if (op1 < 0) creGateBTOR(CKT_BUF_CELL, width, nameFromId(id), nameFromId(-op1));
            else creGateBTOR(CKT_INV_CELL, width, nameFromId(id), nameFromId(op1));
         }
         else if (op == "neg") {  // negative : id neg width op1
            if (op1 < 0) creGateBTOR(CKT_BUF_CELL, width, tempFromId(++tempId), nameFromId(-op1));
            else creGateBTOR(CKT_INV_CELL, width, tempFromId(++tempId), nameFromId(op1));
            creGateBTOR(width, tempFromId(++tempId), "'b" + myInt2Str(1));
            creGateBTOR(CKT_ADD_CELL, width, nameFromId(id), tempFromId(tempId), tempFromId(tempId - 1));
         }
         else if (op == "inc") {  // increment : id inc width op1
            opStr1 = creGateBTOR(op1);
            creGateBTOR(width, tempFromId(++tempId), "'b" + myInt2Str(1));
            creGateBTOR(CKT_ADD_CELL, width, nameFromId(id), opStr1, tempFromId(tempId));
         }
         else if (op == "dec") {  // decrement : id dec width op1
            opStr1 = creGateBTOR(op1);
            creGateBTOR(width, tempFromId(++tempId), "'b" + myInt2Str(1));
            creGateBTOR(CKT_SUB_CELL, width, nameFromId(id), opStr1, tempFromId(tempId));
         }
         else if (op == "redand" || op == "redor" || op == "redxor") {
            if (width != 1) {
               Msg(MSG_ERR) << "Reduced variable " << id << " has width more than 1 !" << endl;
               curCktModule = 0; return;
            }
            opStr1 = creGateBTOR(op1);
            if (op == "redand") // reduced AND : id redand 1 op1
               creGateBTOR(CKT_AND_CELL, 1, nameFromId(id), opStr1);
            else if (op == "redor") // reduced OR : id redor 1 op1
               creGateBTOR(CKT_OR_CELL, 1, nameFromId(id), opStr1);
            else if (op == "redxor") // reduced XOR : id redxor 1 op1
               creGateBTOR(CKT_XOR_CELL, 1, nameFromId(id), opStr1);
            else assert (0);
         }
         else assert (0);
      }
      else if ( // 2 operand operators
            op == "and" || op == "or" || op == "xor" || op == "nand" || op == "nor" || 
            op == "xnor" || op == "add" || op == "sub" || op == "mul" || op == "urem" || 
            op == "udiv" || op == "eq" || op == "ne" || op == "ult" || op == "ulte" ||
            op == "ugt" || op == "ugte" || op == "sll" || op == "srl" || op == "concat"
            )
      {
         if (tokens.size() < 4) {
            Msg(MSG_ERR) << "Missing operand 1 for variable " << id << " (" << op << ") !" << endl;
            curCktModule = 0; return;
         }
         else if (!myStr2Int(tokens[3], op1)) {
            Msg(MSG_ERR) << "Variable " << id << " has non-integer operand \"" << tokens[3] << "\" !" << endl;
            curCktModule = 0; return;
         }
         if (tokens.size() < 5) {
            Msg(MSG_ERR) << "Missing operand 2 for variable " << id << " (" << op << ") !" << endl;
            curCktModule = 0; return;
         }
         else if (!myStr2Int(tokens[4], op2)) {
            Msg(MSG_ERR) << "Variable " << id << " has non-integer operand \"" << tokens[4] << "\" !" << endl;
            curCktModule = 0; return;
         }
         opStr1 = creGateBTOR(op1); opStr2 = creGateBTOR(op2);
         if (op == "and") creGateBTOR(CKT_AND_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "or") creGateBTOR(CKT_OR_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "xor") creGateBTOR(CKT_XOR_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "nand") creGateBTOR(CKT_NAND_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "nor") creGateBTOR(CKT_NOR_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "xnor") creGateBTOR(CKT_XNOR_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "add") creGateBTOR(CKT_ADD_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "sub") creGateBTOR(CKT_SUB_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "mul") creGateBTOR(CKT_MULT_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "udiv") creGateBTOR(CKT_DIV_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "urem") {
            Msg(MSG_ERR) << "Operator \"urem\" has not been implemented !" << endl;
            curCktModule = 0; return;
         }
         else if (op == "sll") creGateBTOR(CKT_SHL_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "srl") creGateBTOR(CKT_SHR_CELL, width, nameFromId(id), opStr1, opStr2);
         else if (op == "concat") creGateBTOR(CKT_MERGE_CELL, width, nameFromId(id), opStr1, opStr2);
         else if ( op == "eq" || op == "ne" || op == "ult" || op == "ulte" || op == "ugt" || op == "ugte") {
            if (width != 1) {
               Msg(MSG_ERR) << "Comparator variable " << id << " has width more than 1 !" << endl;
               curCktModule = 0; return;
            }
            if (op == "eq") creGateBTOR(CKT_EQUALITY_CELL, 1, nameFromId(id), opStr1, opStr2);
            else if (op == "ne") {
               creGateBTOR(CKT_EQUALITY_CELL, 1, tempFromId(++tempId), opStr1, opStr2);
               creGateBTOR(CKT_INV_CELL, 1, nameFromId(id), tempFromId(tempId));
            }
            else if (op == "ult") creGateBTOR(CKT_LESS_CELL, 1, nameFromId(id), opStr1, opStr2);
            else if (op == "ulte") creGateBTOR(CKT_LEQ_CELL, 1, nameFromId(id), opStr1, opStr2);
            else if (op == "ugt") creGateBTOR(CKT_GREATER_CELL, 1, nameFromId(id), opStr1, opStr2);
            else if (op == "ugte") creGateBTOR(CKT_GEQ_CELL, 1, nameFromId(id), opStr1, opStr2);
            else assert (0);
         }
         else assert (0);
      }
      else if ( // More than 2 operands
            op == "cond" || op == "slice"
            )
      {
         if (tokens.size() < 4) {
            Msg(MSG_ERR) << "Missing operand 1 for variable " << id << " (" << op << ") !" << endl;
            curCktModule = 0; return;
         }
         else if (!myStr2Int(tokens[3], op1)) {
            Msg(MSG_ERR) << "Variable " << id << " has non-integer operand \"" << tokens[3] << "\" !" << endl;
            curCktModule = 0; return;
         }
         if (tokens.size() < 5) {
            Msg(MSG_ERR) << "Missing operand 2 for variable " << id << " (" << op << ") !" << endl;
            curCktModule = 0; return;
         }
         else if (!myStr2Int(tokens[4], op2)) {
            Msg(MSG_ERR) << "Variable " << id << " has non-integer operand \"" << tokens[4] << "\" !" << endl;
            curCktModule = 0; return;
         }
         if (tokens.size() < 6) {
            Msg(MSG_ERR) << "Missing operand 3 for variable " << id << " (" << op << ") !" << endl;
            curCktModule = 0; return;
         }
         else if (!myStr2Int(tokens[5], op3)) {
            Msg(MSG_ERR) << "Variable " << id << " has non-integer operand \"" << tokens[5] << "\" !" << endl;
            curCktModule = 0; return;
         }
         if (op == "cond") {  // multiplexer : id cond width op_select op_true op_false
            opStr1 = creGateBTOR(op1); opStr2 = creGateBTOR(op2); opStr3 = creGateBTOR(op3);
            curCktModule->creMuxCell(width, nameFromId(id));
            vector<string> inName; inName.clear();
            inName.push_back(opStr3);
            inName.push_back(opStr2);
            inName.push_back(opStr1);
            curCktModule->crePreOut(nameFromId(id), inName);
         }
         else if (op == "slice") {  // split : id slice width op1 op_msb op_lsb
            opStr1 = creGateBTOR(op1);
            curCktModule->creSplitCell(width, nameFromId(id), op2, op3);
            vector<string> inName; inName.clear();
            inName.push_back(opStr1);
            curCktModule->crePreOut(nameFromId(id), inName);
         }
         else assert (0);
      }
      else {
         Msg(MSG_ERR) << "Unknown or unhandled operator \"" << op << "\" !" << endl;
         curCktModule = 0; return;
      }
   }
}

#endif
