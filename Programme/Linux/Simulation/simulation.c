#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../config.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <fcntl.h>

#include <errno.h>

int main()
{
	int bal_log_disque = mq_open( "/bal_disque", O_WRONLY );
	printf("%i\n", bal_log_disque);
	char commande[20];
	for( ; ; )
	{
		scanf("%s", commande);
		if( !strcmp(commande, "logd") )
		{
			mq_send(bal_log_disque, "Hello", sizeof(log_t), 1);
		}
		if( !strcmp(commande, "logdf") )
		{
			mq_send(bal_log_disque, "Fin", sizeof(log_t), 2 );
		}
		if( !strcmp(commande, "q") )
		{
			break;
		}
	}
	return 0;
}