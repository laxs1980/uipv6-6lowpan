#ifndef _UDP_DEV_H_
#define _UDP_DEV_H_

#include <stdint.h>

#include "contiki-net.h"

void udpclidev_init(void);
uint16_t udpclidev_poll(void);
uint8_t udpcli_send(uip_lladdr_t *lladdr);

#endif /* _UDP_DEV_H_ */

