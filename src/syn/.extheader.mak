syn.d: ../../include/synVar.h 
../../include/synVar.h: synVar.h
	@rm -f ../../include/synVar.h
	@ln -fs ../src/syn/synVar.h ../../include/synVar.h
