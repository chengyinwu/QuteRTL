PKGFLAG = -D_WITH_BDD_DEBUG
EXTHDRS = VLGlobe.h vlpBase.h vlpDefine.h vlpEnum.h vlpItoStr.h vlpOpNode.h vlpSubElement.h vlpCtrlNode.h vlpDesign.h vlpHierTree.h vlpModule.h vlpStack.h y.tab.h lib.tab.h parser.h vlpCmd.h vlpCU.h vlpFSM.h
 
LEX  = flex
YACC = bison
YFLAG = -d -v

YYTABC = vlpYacc.tab.c
YYTABH = vlpYacc.tab.h
YYTABC1 = vlpLibYacc.tab.c
YYTABH1 = vlpLibYacc.tab.h

include ../Makefile.in
include ../Makefile.lib

cleanly:
	@rm -f lex.yy.cpp
	@rm -f lex.lib.cpp
	@rm -f y.tab.*
	@rm -f lib.tab.*

lex.yy.cpp:vlpLex.l y.tab.h
	@echo "> lex: $<"
	@$(LEX) vlpLex.l
	@mv lex.yy.c lex.yy.cpp

lex.lib.cpp:vlpLibLex.l lib.tab.h
	@echo "> lex: $<"
	@$(LEX) -Plib vlpLibLex.l
	@mv lex.lib.c lex.lib.cpp

y.tab.cpp y.tab.h:vlpYacc.y
	@echo "> Yacc: $<"
	@$(YACC) $(YFLAG) vlpYacc.y
	@mv $(YYTABC) y.tab.cpp
	@mv $(YYTABH) y.tab.h

lib.tab.cpp lib.tab.h:vlpLibYacc.y
	@echo "> Yacc: $<"
	@$(YACC) $(YFLAG) -p lib  vlpLibYacc.y
	@mv $(YYTABC1) lib.tab.cpp
	@mv $(YYTABH1) lib.tab.h

