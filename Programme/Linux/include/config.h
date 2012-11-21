#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>

/*Port d'ecoute du serveur*/
#define NUM_PORT 32768

/*Constante générales au programme*/
#define MAX_CARTON_FILE 10
#define CARTON_PLEIN 1
#define PALETTE_PLEINE 2
#define TAILLE_ENTREPOT 20
#define NB_THREAD_AU 2
#define MODERW 0666
#define TRAME_FIN "Fin"
#define MESSAGE_TYPE_STD 1
#define MESSAGE_TYPE_FIN 2

/*Priorites des boites aux lettres*/
#define BAL_PRIO_FIN 2
#define BAL_PRIO_ELSE 1

/*Constantes relatives à la memoire partagee statut*/
#define STATUT_SIZE 7
#define ST_PRESENCE_CARTON 0
#define ST_PRESENCE_PALETTE 1
#define ST_IMPRIMANTE 2
#define ST_PIECE 3
#define ST_FILM 4
#define ST_CLAPET_OUVERT 5
#define ST_NB_ERREUR 6

/*Constante definissant l'absence de palette*/
#define NO_PALETTE -1

/*Priorite des threads*/
#define PRIO_MERE 10
#define PRIO_ERREUR 9
#define PRIO_ENVOI_PIECE 8
#define PRIO_CARTON 7
#define PRIO_LOG_WINDOWS 6
#define PRIO_PALETTE 5
#define PRIO_CARISTE 4
#define PRIO_COMMANDE_WINDOWS 3
#define PRIO_LOG_DISQUE 2
#define PRIO_SIMULATION 1
		
/*Types généraux au programme*/
typedef char log_t[128];
typedef int lot_t[3];
/*lot[0] = nb palette A / lot[1] = nb palette B / lot[2] = max rebus*/
typedef int statut_t[STATUT_SIZE];
typedef char erreur_t[8];

/*Constantes definissant le type de piece*/
#define LOT_A 0
#define LOT_B 1
#define REBUS 2

/*Constantes identifiant les erreurs*/
#define ERR_AU 0
#define ERR_TROP_DE_REBUS 1
#define ERR_PAS_DE_CARTON 2
#define ERR_IMPRIMANTE_KO 3
#define ERR_FILE_D_ATTENTE 4
#define ERR_PAS_DE_PALETTE 5
#define ERR_FILM_KO 6

/*Structures associees aux memoires partagee*/
typedef struct palette {
	int id;
	char type;
	char heure[6];
} palette_t;

typedef struct entrepot {
	palette_t palettes[TAILLE_ENTREPOT];
} entrepot_t;

/*Nom des boites aux lettres*/
#define BALERR "/bal_erreur"
#define BALDIS "/bal_disque"
#define BALWIN "/bal_windows"

/*Nom du fichier de log*/
#define NOM_LOG "log.txt"

#endif
