<div align="center">
<b>QuteRTL-20111020 Source Release</b>
<br/>
<b>Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan</b>
<br/><br/>
</div>

### LATEST NOTE

#### This is a mirror of the source code in our first release in 2011 and thus it's just for those who want to use QuteRTL right now.

Unfortunately our server has been terribly crashed for a long time and it's still under repair.
Once the server is up we'll update the code here to our latest release.
Please contact us if you need extra materials.

## INTRODUCTION

QuteRTL is a word-level framework developed for years. It generally supports synthesizable Verilog input and BLIF format designs, followed by a quick synthesizer which enables synthesizing CDFG (Control Data Flow Graph) into word-level netlist, including logics, arithmetics, shifters, comparators, FFs ..., etc. Afterwards, it supports us to flatten the hierarchical design into single flattened module. Output format can be either Verilog (hierarchical or flattened) or BLIF. Other than the main synthesis flow on RTL design, QuteRTL front-end also provides several helpful functionalities in helping understanding user the design.

In this version of release, we provide our source code for RTL front-end parser, synthesizer, circuit flatten, and design output writer. Commands for front-end manipulation are also involved. Please refer to our website for detailed information.

## DOWNLOAD

Latest QuteRTL source code can be download in our [website](http://dvlab.ee.ntu.edu.tw/~publication/QuteRTL/index.html)

Alternatively you can clone this repository: `git clone https://github.com/chengyinwu/QuteRTL`

### COMPILE

```bash
$ cd <root-directory-of-QuteRTL>
$ make clean; make
```

### EXECUTE

```bash
$ cd <root-directory-of-QuteRTL>
$ ./qutertl
```

## COMMANDS

Type "help" in QuteRTL to get a full list of supported commands.

```bash
# Common Commands
DOfile                    # Execute the commands in the dofile
HELp                      # Print this help message
HIStory                   # Print command history
Quit                      # Quit the execution
SET LOgfile               # Copy and/or redirect messages to assigned file
USAGE                     # Report the runtime and / or memory usage

# I/O Commands
REAd DEsign               # Read design or the filelist of designs
WRIte CKt                 # Write out flattened Verilog or BLIF circuit
WRIte DEsign              # Write out hierarchical Verilog design

# Report Commands
REPort CDfg               # Report assigned node's CDFG information
REPort CKt                # Report flattened circuit information
REPort DEsign             # Report hierarchical design information
REPort GAte               # Report assigned gate's information
REPort MOdule             # Report module information

# Print Commands
WRIte CDfg                # Write the CDFG of assigned node before synthesis
WRIte CFG                 # Write out the control-flow graph for a specific pin with assigned bits
WRIte DFG                 # Write out the data-flow graph of assigned gate in flattened design
WRIte HIErarchy           # Write design hierarchical relationship

# Design Flow Commands
FLAtten                   # Perform flatten on synthesized design
SYNthesis                 # Perform synthesis on parsed RTL design

# Ckt Construction Commands
CHAnge MODule             # Change current module to its parant module in current design hierarchy
CREate DEsign             # Create design from command line interface
DEFine CEll               # Create cell(s) with specified cell type on current design
DEFine INST               # Create module instance
DEFine MODule             # Create a new sub-module on current design
DEFine NET                # Create I/O ports and nets on current design
```

## LICENSE

Please refer to the [COPYING](COPYING) file for license and copying information.

## AUTHOR LIST
* [Hu-Hsi Yeh](mailto:f93943122@ntu.edu.tw)
* [Cheng-Yin Wu](mailto:chengyinwu123@gmail.com)
* [Chung-Yang (Ric) Huang](mailto:ric@cc.ee.ntu.edu.tw)
