#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "contiki-net.h"
#include "net/netstack.h"
#include "net/mac/rdc.h"
#include "net/tcpip.h"
#include "net/uip-ds6.h"
#include "cpu/native/net/tapdev6.h"
#include "cpu/native/net/udpdev.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

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

  /* Start radio and radio receive process */
  NETSTACK_RADIO.init();

  /* Initialize stack protocols */
  queuebuf_init();
  NETSTACK_RDC.init();
  NETSTACK_MAC.init();
  NETSTACK_NETWORK.init();

  process_start(&tcpip_process, NULL);

  printf("Enter to main loop\n");

	while(1){
		process_run();
		etimer_request_poll();
	}
}

void uip_log(char *m)
{
	printf("%s\n", m);
}

