#ifndef _SC16IS7X0_H_
#define _SC16IS7X0_H_

#include <stdio.h>
#include "setting_structs.h"
#include "comm_interface.h"
#include "commands.h"
#include "lpc17xx.h"
#include "board.h"

//! IRQ-pin
#define SC16IS740_IRQ_PIN   (1<<SC16IS7X0_IRQ)

//! The clock frequency of the XTAL in Hz
#define SC16IS7X0_XTAL_FREQ 14745600

//! Low register for the baud rate - 1200 baud
#define SC16IS7X0_BAUD_DLL  0x00
//! High register for the baud rate - 1200 baud
#define SC16IS7X0_BAUD_DLH  0x03

//! Receive Holding Register
#define SC16IS7X0_RHR     0x00 << 3
//! Transmit Holding Register
#define SC16IS7X0_THR     0x00 << 3
//! Interrupt Enable Register
#define SC16IS7X0_IER     0x01 << 3
//! Interrupt Identification Register
#define SC16IS7X0_IIR     0x02 << 3
//! FIFO Control Register
#define SC16IS7X0_FCR     0x02 << 3
//! Line Control Register
#define SC16IS7X0_LCR     0x03 << 3
//! Modem Control Register
#define SC16IS7X0_MCR     0x04 << 3
//! Line Status Register
#define SC16IS7X0_LSR     0x05 << 3
//! Modem Status Register
#define SC16IS7X0_MSR     0x06 << 3
//! Scratchpad Register
#define SC16IS7X0_SPR     0x07 << 3
//! Transmission Control Register
#define SC16IS7X0_TCR     0x06 << 3
//! Trigger Level Register
#define SC16IS7X0_TLR     0x07 << 3
//! Transmit FIFO Level Register
#define SC16IS7X0_TXLVL   0x08 << 3
//! Receive FIFO Level Register
#define SC16IS7X0_RXLVL   0x09 << 3
//! I/O pin Direction Register
#define SC16IS7X0_IODIR   0x0A << 3
//! I/O pin States Register
#define SC16IS7X0_IOSTATE 0x0B << 3
//! I/O Interrupt Enable Register
#define SC16IS7X0_IOINTEN 0x0C << 3
//! I/O pins Control Register
#define SC16IS7X0_IOCTRL  0x0E << 3
//! Extra Features Register
#define SC16IS7X0_EFCR    0x0F << 3

//! Divisor latch LSB (DLL) - Only accessible when LCR[7] = 1 and not 0xBF
#define SC16IS7X0_DLL     0x00 << 3
//! Divisor latch MSB (DLH) - Only accessible when LCR[7] = 1 and not 0xBF
#define SC16IS7X0_DLH     0x01 << 3
//! Enhanced Feature Register (EFR) - Only accessible when LCR = 0xBF
#define SC16IS7X0_EFR     0x02 << 3

/*! \brief Initialize the chip
 *  \param addr The chips I2C address
 *  \return 1 if the device failed, 0 if not */
uint8_t sc16is7x0_init(void);

/*! \brief This function should be called each ms */
void sc16is7x0_1ms_tick(void);

void sc16is7x0_write_char(uint8_t data);
void sc16is7x0_write_data(uint8_t length, uint8_t *data);
uint8_t sc16is7x0_set_baudrate();

uint8_t sc16is7x0_uart_connected(void);

int16_t sc16is7x0_poll_char(void);

uint8_t sc16is7x0_read_byte(void);
uint8_t sc16is7x0_data_available(void);

void sc16is7x0_reset_rx_fifo(void);
void sc16is7x0_reset_tx_fifo(void);

#endif
