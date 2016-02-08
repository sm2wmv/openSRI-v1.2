#ifndef __COMPUTER_INTERFACE_H
#define __COMPUTER_INTERFACE_H

#include <stdlib.h>
#include <stdio.h>

#include "comm_interface.h"
#include "lpc17xx_uart.h"

void computer_interface_uart_rx(struct_comm_interface_msg message);
void computer_interface_uart_tx(uint8_t data);
uint32_t computer_interface_uart_poll_rx(uint8_t *buffer);

uint8_t computer_interface_connection_status(void);
void computer_interface_disconnected_event(void);
void computer_interface_pong_event(void);
void computer_interface_connected_event(void);
void computer_interface_1ms_tick(void);
void computer_interface_execute();

#endif /* end __COMPUTER_INTERFACE_H */

/****************************************************************************
**                            End Of File
*****************************************************************************/
