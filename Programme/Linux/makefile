CCFLAGS=-Wall
LDFLAGS=
RMFLAGS=-f -R

INCDIR=include
SRCDIR=src
BLDDIR=build

SYSLIBS=-lpthread -lrt
PERSLIBS=-I$(INCDIR)/

COMP=gcc -c
LINK=gcc -o

EXE=CIAI

OBJ=mere.o log_disque.o simulation.o

$(EXE) : $(OBJ)
	echo Linking $(EXE)
	rm $(RMFLAGS) $(BLDDIR)
	mkdir $(BLDDIR)
	$(LINK) $(LDFLAGS) $(BLDDIR)/$(EXE) $(OBJ) $(PERSLIBS) $(SYSLIBS)

%.o : $(SRCDIR)/%.c $(INCDIR)/%.h
	echo Compiling $<
	$(COMP) $(CCFLAGS) $(PERSLIBS) $(SYSLIBS) $<

simulation.o : $(SRCDIR)/simulation.c $(INCDIR)/simulation.h $(INCDIR)/config.h

log_disque.o : $(SRCDIR)/log_disque.c $(INCDIR)/log_disque.h $(INCDIR)/config.h

mere.o : $(SRCDIR)/mere.c $(INCDIR)/config.h
	gcc -c $(SRCDIR)/mere.c $(SYSLIBS) $(PERSLIBS)

clean :
	echo Cleaning...
	rm $(RMFLAGS) $(OBJ) $(BLDDIR)