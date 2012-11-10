#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>

typedef char log_t[128];
typedef int lot_t[2];
typedef char statut_t;

struct erreur {
	int id;
	char heure[6];
};

struct palette {
	int id;
	char type;
	char heure[6];
};

struct entrepot {
	struct palette palettes[20];
};

extern const mode_t MODERW;
extern const log_t TRAME_FIN;
extern const int MESSAGE_TYPE_STD;
extern const int MESSAGE_TYPE_FIN;

extern const char * BALERR;
extern const char * BALDIS;
extern const char * BALWIN;

extern const char * NOM_LOG;

#endif
