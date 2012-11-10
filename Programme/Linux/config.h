#include <sys/types.h>

const mode_t MODERW = 0666;

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

