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

OBJ=mere.o log_disque.o log_windows.o simulation.o erreur.o commande_windows.o prod_utils.o carton.o

$(EXE) : $(OBJ)
	echo Linking $(EXE)
	rm $(RMFLAGS) $(BLDDIR)
	mkdir $(BLDDIR)
	$(LINK) $(LDFLAGS) $(BLDDIR)/$(EXE) $(OBJ) $(PERSLIBS) $(SYSLIBS)

production : prod_utils.o carton.o palette.o cariste.o

%.o : $(SRCDIR)/%.c $(INCDIR)/%.h
	echo Compiling $<
	$(COMP) $(CCFLAGS) $(PERSLIBS) $(SYSLIBS) $<

simulation.o : $(SRCDIR)/simulation.c $(INCDIR)/simulation.h $(INCDIR)/config.h

log_disque.o : $(SRCDIR)/log_disque.c $(INCDIR)/log_disque.h $(INCDIR)/config.h

log_windows.o : $(SRCDIR)/log_windows.c $(INCDIR)/log_windows.h $(INCDIR)/config.h

erreur.o : $(SRCDIR)/erreur.c $(INCDIR)/config.h
	$(COMP) $(CCFLAGS) $(SRCDIR)/erreur.c $(SYSLIBS) $(PERSLIBS)

commande_windows.o : $(SRCDIR)/commande_windows.c $(INCDIR)/commande_windows.h $(INCDIR)/config.h

prod_utils.o : $(SRCDIR)/prod_utils.c $(INCDIR)/prod_utils.h $(INCDIR)/config.h

carton.o : $(SRCDIR)/carton.c $(INCDIR)/prod_utils.h $(INCDIR)/config.h
	$(COMP) $(CCFLAGS) $(SRCDIR)/carton.c $(SYSLIBS) $(PERSLIBS) 

palette.o : $(SRCDIR)/palette.c $(INCDIR)/prod_utils.h $(INCDIR)/config.h
	$(COMP) $(CCFLAGS) $(SRCDIR)/palette.c $(SYSLIBS) $(PERSLIBS)

cariste.o : $(SRCDIR)/cariste.c $(INCDIR)/prod_utils.h $(INCDIR)/config.h
	$(COMP) $(CCFLAGS) $(SRCDIR)/cariste.c $(SYSLIBS) $(PERSLIBS)

mere.o : $(SRCDIR)/mere.c $(INCDIR)/config.h
	$(COMP) $(CCFLAGS) $(SRCDIR)/mere.c $(SYSLIBS) $(PERSLIBS)

clean :
	echo Cleaning...
	rm $(RMFLAGS) $(OBJ) $(BLDDIR)
