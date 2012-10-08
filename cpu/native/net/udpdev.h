#ifndef _UDP_DEV_H_
#define _UDP_DEV_H_

#include <stdint.h>

#include "contiki-net.h"

void udpdev_init(void);
uint16_t udpdev_poll(void);
uint8_t udpdev_send(uip_lladdr_t *lladdr);

#endif /* _UDP_DEV_H_ */

