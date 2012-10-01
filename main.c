#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "net/tcpip.h"

int main(int argc, char **argv)
{
	printf("uIPv6 test project\n");
	
	printf("Process subsystem init\n");
	process_init();
	printf("Ok\n");

	printf("Start etimer process\n");
	process_start(&etimer_process, NULL);
	printf("Ok\n");

	printf("Init ctimer\n");
	ctimer_init();
	printf("Ok\n");

	printf("Start tcpip process\n");
	process_start(&tcpip_process, NULL);
	printf("Ok\n");

	while(1){
		process_run();
		etimer_request_poll();
	}

	return 0;
}

void uip_log(char *m)
{
	printf("%s\n", m);
}
