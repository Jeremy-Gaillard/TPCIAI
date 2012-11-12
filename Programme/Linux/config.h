#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>

#define CARTON_PLEIN 10
#define MAX_REBUS 10

typedef char log_t[128];
typedef int lot_t[2];
typedef char statut_t;

typedef struct erreur {
	int id;
	char heure[6];
} erreur_t;

typedef struct palette {
	int id;
	char type;
	char heure[6];
} palette_t;

typedef struct entrepot {
	palette_t palettes[20];
} entrepot_t;

extern const mode_t MODERW;
extern const log_t TRAME_FIN;
extern const int MESSAGE_TYPE_STD;
extern const int MESSAGE_TYPE_FIN;

extern const char * BALERR;
extern const char * BALDIS;
extern const char * BALWIN;

extern const char * NOM_LOG;

#endif
