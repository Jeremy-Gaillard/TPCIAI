CONFIGDIR=.
BLDDIR=build
OBJDIR=obj
EXE=CIAI

PRODDIR=lot_production
PRODSRC=$(PRODDIR)/carton.c
PRODOBJ=$(PRODDIR)/carton.o
PRODUTILS=$(PRODDIR)/prod_utils.c $(PRODDIR)/prod_utils.h
GLOBSRC=mere.c log_disque.c
GLOBOBJ=mere.o log_disque.o

OBJ=$(PRODOBJ) $(GLOBOBJ)
SRC=$(PRODSRC) $(GLOBSRC)

COMP=gcc -c
LINK=gcc -o

LIBS=-lpthread -lrt
CCFLAGS=-Wall


mere.o :
	$(COMP) $(CCFLAGS) $(LIBS) -I$(CONFIGDIR) mere.c config.h
log_disque.o : log_disque.c log_disque.h config.h
# production : carton.o
carton.o :
	$(COMP) $(CCFLAGS) $(LIBS) -I$(CONFIGDIR) $(PRODDIR)/carton.c $(PRODUTILS) config.h


