cmd.d: ../../include/cmdDesign.h ../../include/cmdMsgFile.h ../../include/cmdParser.h 
../../include/cmdDesign.h: cmdDesign.h
	@rm -f ../../include/cmdDesign.h
	@ln -fs ../src/cmd/cmdDesign.h ../../include/cmdDesign.h
../../include/cmdMsgFile.h: cmdMsgFile.h
	@rm -f ../../include/cmdMsgFile.h
	@ln -fs ../src/cmd/cmdMsgFile.h ../../include/cmdMsgFile.h
../../include/cmdParser.h: cmdParser.h
	@rm -f ../../include/cmdParser.h
	@ln -fs ../src/cmd/cmdParser.h ../../include/cmdParser.h
