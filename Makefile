# source codes to be compiled
SRCPKGS  = cmd vlp syn ckt util

# library to be linked (should include SRCPKGS)
LIBPKGS  = cmd vlp syn ckt util

MAIN     = main

LIBS     = $(addprefix -l, $(LIBPKGS))
SRCLIBS  = $(addsuffix .a, $(addprefix lib, $(SRCPKGS)))
LIBFILES = $(addsuffix .a, $(addprefix lib, $(LIBPKGS)))

EXEC     = qutertl
AR       = ar cr

all: srcLib
	@echo "Checking $(MAIN)..."
	@cd src/$(MAIN); make --no-print-directory INCLIB="$(LIBS)" EXEC=$(EXEC); cd ../.. ;
	@ln -fs bin/$(EXEC) .
#	@strip bin/$(EXEC)
       	
srcLib: 
	@for pkg in $(SRCPKGS); \
	do \
		echo "Checking $$pkg..."; \
		cd src/$$pkg; make --no-print-directory PKGNAME=$$pkg; \
		cd ../.. ; \
	done

clean:
	@for pkg in $(SRCPKGS); \
	do \
		echo "Cleaning $$pkg..."; \
		cd src/$$pkg; make --no-print-directory PKGNAME=$$pkg clean; \
		cd ../.. ; \
	done
	@echo "Cleaning $(MAIN)..."
	@cd src/$(MAIN); make --no-print-directory clean
	@echo "Removing $(LIBFILES)..."
	@cd lib; rm -f $(LIBFILES)
	@echo "Removing $(EXEC)..."
	@rm -f bin/$(EXEC) 
	@echo "Removing obj/"
	@cd obj/; rm -rf *.o

ctags:	  
	@rm -f src/tags
	@for pkg in $(SRCPKGS); \
	do \
		echo "Tagging $$pkg..."; \
		cd src; ctags -a $$pkg/*.cpp $$pkg/*.h; cd ..; \
	done
	@echo "Tagging $(MAIN)..."
	cd src; ctags -a $(MAIN)/*.cpp
