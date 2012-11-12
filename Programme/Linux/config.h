#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>

#define CARTON_PLEIN 10
#define PALETTE_PLEINE 4
#define MAX_REBUS 10

#define BAL_PRIO_ERREUR 1
#define BAL_PRIO_ELSE 2

#define STATUT_SIZE 6
#define ST_PRESENCE_CARTON 0
#define ST_PRESENCE_PALETTE 1
#define ST_IMPRIMANTE 2
#define ST_PIECE 3
#define ST_FILM 4
#define ST_CLAPET_OUVERT 5

typedef char log_t[128];
typedef int lot_t[2];
typedef int statut_t[STATUT_SIZE];
/* mommy, could you set all my ints to 0 please? */

#define LOT_A 0
#define LOT_B 1

#define ERR_AU 0
#define ERR_TROP_DE_REBUS 1
#define ERR_PAS_DE_CARTON 2
#define ERR_IMPRIMANTE_KO 3
#define ERR_FILE_D_ATTENTE 4
#define ERR_PAS_DE_PALETTE 5
#define ERR_FILM_KO 6

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
