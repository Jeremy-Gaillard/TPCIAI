#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>

#define CARTON_PLEIN 10
#define MAX_REBUS 10

#define STATUT_SIZE 6
#define ST_PRESENCE_CARTON 0
#define ST_PRESENCE_PALETTE 1
#define ST_IMPRIMANTE 2
#define ST_PIECE_OK 3
#define ST_FILM_OK 4
#define ST_CLAPET_OUVERT 5

typedef char log_t[128];
typedef int lot_t[2];
typedef int statut_t[STATUT_SIZE];

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
