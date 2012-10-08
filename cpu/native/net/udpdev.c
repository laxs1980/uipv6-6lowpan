#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include "contiki-net.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
#define IPBUF ((struct uip_tcpip_hdr *)&uip_buf[UIP_LLH_LEN])

int sockfd,n;
struct sockaddr_in servaddr,cliaddr;
char sendline[1000];
char recvline[1000];

void udpdev_init(void)
{
	char *env_s_ip;
	char *env_s_port;
	char *env_c_ip;
	char *env_c_port;
	char *env_ll_addr;

	env_s_ip = getenv ("UDPDEV_S_IP");
	if (env_s_ip == NULL){
		printf ("udpdev# UDPDEV_S_IP not found\n");
		exit(1);
	}

	printf("udpdev# serv ip is: %s\n", env_s_ip);

	env_s_port = getenv ("UDPDEV_S_PORT");
	if (env_s_port == NULL){
		printf ("udpdev# UDPDEV_S_PORT not found\n");
		exit(1);
	}

	printf("udpdev# serv port is: %s\n", env_s_port);

	env_c_ip = getenv ("UDPDEV_C_IP");
	if (env_c_ip == NULL){
		printf ("udpdev# UDPDEV_C_IP not found\n");
		exit(1);
	}

	printf("udpdev# cli ip is: %s\n", env_c_ip);

	env_c_port = getenv ("UDPDEV_C_PORT");
	if (env_c_port == NULL){
		printf ("udpdev# UDPDEV_C_PORT not found\n");
		exit(1);
	}

	printf("udpdev# cli port is: %s\n", env_c_port);

	env_ll_addr = getenv ("UDPDEV_LL_ADDR");
	if (env_ll_addr == NULL){
		printf ("udpdev# UDPDEV_LL_ADDR not found\n");
		exit(1);
	}

	printf("udpdev# lladdr port is: %s\n", env_ll_addr);

	uip_lladdr.addr[5] = atoi(env_ll_addr);

	sockfd=socket(AF_INET,SOCK_DGRAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(env_s_ip);
	servaddr.sin_port=htons(atoi(env_s_port));
	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	bzero(&cliaddr,sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr=inet_addr(env_c_ip);
	cliaddr.sin_port=htons(atoi(env_c_port));

	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags < 0){
		printf("udpdev# fcntl error\n");
		exit(1);
	}
	flags = (flags|O_NONBLOCK);
	fcntl(sockfd, F_SETFL, flags);
}

uint16_t udpdev_poll(void)
{
	int ret = 0;
	ret = recvfrom(sockfd, uip_buf, UIP_BUFSIZE, 0, NULL, NULL);
	//if(ret == -1){
	//	printf("udpdev# recv error: %d\n", errno);
	//	exit(1);
	//}
	if(ret > 0){
		printf("udpdev# receive msg\n");
		return ret;
	}
	return 0;
}

uint8_t udpdev_send(uip_lladdr_t *lladdr)
{
  /*
   * If L3 dest is multicast, build L2 multicast address
   * as per RFC 2464 section 7
   * else fill with th eaddrsess in argument
   */
  if(lladdr == NULL) {
    /* the dest must be multicast */
    (&BUF->dest)->addr[0] = 0x33;
    (&BUF->dest)->addr[1] = 0x33;
    (&BUF->dest)->addr[2] = IPBUF->destipaddr.u8[12];
    (&BUF->dest)->addr[3] = IPBUF->destipaddr.u8[13];
    (&BUF->dest)->addr[4] = IPBUF->destipaddr.u8[14];
    (&BUF->dest)->addr[5] = IPBUF->destipaddr.u8[15];
  } else {
    memcpy(&BUF->dest, lladdr, UIP_LLADDR_LEN);
  }
  memcpy(&BUF->src, &uip_lladdr, UIP_LLADDR_LEN);
  BUF->type = UIP_HTONS(UIP_ETHTYPE_IPV6); //math tmp
   
  uip_len += sizeof(struct uip_eth_hdr);
 
  int ret = sendto(sockfd, uip_buf, uip_len, 0,
		  (struct sockaddr *)&cliaddr, sizeof(cliaddr));

  if(ret == -1){
	  printf("udpdev# sendto error\n");
	  exit(1);
  }

  return 0;
}

