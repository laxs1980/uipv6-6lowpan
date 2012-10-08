#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "net/tcpip.h"
#include "net/uip-ds6.h"
#include "cpu/native/net/tapdev6.h"
#include "cpu/native/net/udpdev.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

PROCESS(tcp_server, "TCP server");

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(tcp_server, ev, data)
{
	PROCESS_BEGIN();

	tcp_listen(UIP_HTONS(8080));

	while(1) {
		PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
		printf("TCP server: receive TCP event\n");

		if(uip_closed() || uip_aborted() || uip_timedout()) {
			printf("TCP server: connection closed\n");
		} else if(uip_connected()) {
			printf("TCP server: connected\n");
		}

		if(uip_newdata()) {
			printf("TCP server: receive new data\n");
			uint16_t len = uip_datalen();
			uint8_t *ptr = uip_appdata;
			while(len--){
				printf("%c", *ptr++);
			}
			printf("\n");
		}
		if(uip_rexmit() ||
				uip_newdata() ||
				uip_acked() ||
				uip_connected() ||
				uip_poll()) {
			//senddata();
		}
	}
  
  PROCESS_END();
}

int main(int argc, char **argv)
{
	printf("uIPv6 test project\n");

	printf("Init udpdevcli\n");
	udpdev_init();
	printf("Ok\n");

    uip_ipaddr_t ipaddr;
    uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
    if((ipaddr.u16[0] != 0) ||
       (ipaddr.u16[1] != 0) ||
       (ipaddr.u16[2] != 0) ||
       (ipaddr.u16[3] != 0)) {

      uip_ds6_prefix_add(&ipaddr, UIP_DEFAULT_PREFIX_LEN, 0);

      uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
      uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
    }
	
	printf("Process subsystem init\n");
	process_init();
	printf("Ok\n");

	printf("Start etimer process\n");
	process_start(&etimer_process, NULL);
	printf("Ok\n");

	printf("Init ctimer\n");
	ctimer_init();
	printf("Ok\n");

//	printf("Init tapdev\n");
//	tapdev_init();
//	tcpip_set_outputfunc(tapdev_send);
//	printf("Ok\n");

	printf("Start tcpip process\n");
	tcpip_set_outputfunc(udpdev_send);
	process_start(&tcpip_process, NULL);
	printf("Ok\n");

	printf("Start TCP server on 8080 port\n");
	process_start(&tcp_server, NULL);
	printf("Ok\n");

	uint8_t i;
	for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
		if(uip_ds6_if.addr_list[i].isused) {
			printf("IPV6 Addresss: ");
			printf("%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
					uip_ds6_if.addr_list[i].ipaddr.u8[0],
					uip_ds6_if.addr_list[i].ipaddr.u8[1],
					uip_ds6_if.addr_list[i].ipaddr.u8[2],
					uip_ds6_if.addr_list[i].ipaddr.u8[3],
					uip_ds6_if.addr_list[i].ipaddr.u8[4],
					uip_ds6_if.addr_list[i].ipaddr.u8[5],
					uip_ds6_if.addr_list[i].ipaddr.u8[6],
					uip_ds6_if.addr_list[i].ipaddr.u8[7],
					uip_ds6_if.addr_list[i].ipaddr.u8[8],
					uip_ds6_if.addr_list[i].ipaddr.u8[9],
					uip_ds6_if.addr_list[i].ipaddr.u8[10],
					uip_ds6_if.addr_list[i].ipaddr.u8[11],
					uip_ds6_if.addr_list[i].ipaddr.u8[12],
					uip_ds6_if.addr_list[i].ipaddr.u8[13],
					uip_ds6_if.addr_list[i].ipaddr.u8[14],
					uip_ds6_if.addr_list[i].ipaddr.u8[15]);
			printf("\n");
		}
	}

	while(1){
		process_run();
		etimer_request_poll();
//		uip_len = tapdev_poll();

		uip_len = udpdev_poll();

		if(uip_len > 0){
			if(BUF->type == uip_htons(UIP_ETHTYPE_IPV6)){
				tcpip_input();
			}
		}
		fflush(stdout);
	}
}

void uip_log(char *m)
{
	printf("%s\n", m);
}

