#ifndef __INIT_H
#define __INIT_H

#define USB_CONNECT_GPIO_PORT_NUM     2
#define USB_CONNECT_GPIO_BIT_NUM      9

void init_io(void);
void init_qei(void);

void init_uart0(uint32_t baudrate);
void init_uart3(uint32_t baudrate);

#endif /* end __INIT_H */

/****************************************************************************
**                            End Of File
*****************************************************************************/
