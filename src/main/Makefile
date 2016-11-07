PKGFLAG   =
EXTHDRS   = 
UIFLAG    =

include ../Makefile.in

BINDIR    = ../../bin
TARGET    = $(BINDIR)/$(EXEC)
#RLFLAG	  = -lreadline -ltermcap

target: $(TARGET)

$(TARGET): $(COBJS) $(LIBDEPEND)
	@echo "> building $(EXEC)..."
	$(CXX) $(CFLAGS) $(UIFLAG) -I$(EXTINCDIR) $(COBJS) $(RLFLAG) -L$(LIBDIR) $(INCLIB) -lreadline -ltermcap -o $@
