#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "sc16is7x0.h"
#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"
#include "misc.h"

static uint16_t ms_counter = 0;

uint8_t sc16is7x0_set_reg(uint8_t reg, uint8_t val) {
  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = SC16IS740_ADDR;
  transferMCfg.retransmissions_max = 1;
  uint8_t tx_data[2];

  tx_data[0] = reg;
  tx_data[1] = val;

  transferMCfg.tx_data = tx_data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;

  return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t sc16is7x0_read_reg(uint8_t reg) {
  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = SC16IS740_ADDR;
  transferMCfg.retransmissions_max = 1;
  uint8_t rx_data[2];

  rx_data[0] = reg;

  transferMCfg.tx_data = rx_data;
  transferMCfg.tx_length = 1;
  transferMCfg.rx_data = rx_data;
  transferMCfg.rx_length = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  return(rx_data[0]);
}

uint8_t sc16is7x0_set_baudrate(void) {
  PRINTF("SC16IS740 RETVAL: %i\n",sc16is7x0_set_reg(SC16IS7X0_LCR, 0x80));
  sc16is7x0_set_reg(SC16IS7X0_DLL, 0x00);
  sc16is7x0_set_reg(SC16IS7X0_DLH, 0x03);

  return(0);
}

uint8_t sc16is7x0_uart_connected(void) {
  //Test to set the temporary register and read the result to see if
  //the UART is connected
  sc16is7x0_set_reg(SC16IS7X0_SPR, 0x55);

  if (sc16is7x0_read_reg(SC16IS7X0_SPR) != 0x55)
    return(1);

  return(0);
}

uint8_t sc16is7x0_init(void) {
  sc16is7x0_set_baudrate();
  sc16is7x0_set_reg(SC16IS7X0_LCR, 0x03);
  sc16is7x0_set_reg(SC16IS7X0_FCR, 0x06);
  sc16is7x0_set_reg(SC16IS7X0_FCR, 0x01);

  return(0);
}

uint8_t sc16is7x0_data_available(void) {
  return(sc16is7x0_read_reg(SC16IS7X0_RXLVL));
}

void sc16is7x0_write_char(uint8_t data) {
  while(sc16is7x0_read_reg(SC16IS7X0_TXLVL) == 0) {
  };

  sc16is7x0_set_reg(SC16IS7X0_THR, data);
}

uint8_t sc16is7x0_read_byte(void) {
  return(sc16is7x0_read_reg(SC16IS7X0_RHR));
}

int16_t sc16is7x0_poll_char(void) {
  if (sc16is7x0_data_available() != 0) {
    uint8_t rx_data[2];

    uint8_t reg_val = sc16is7x0_read_reg(SC16IS7X0_LSR);

    if (reg_val & (1<<7))
      PRINTF("ERROR: SC16IS740 FIFO DATA\n\r");
    if (reg_val & (1<<3))
      PRINTF("ERROR: SC16IS740 FRAMING\n\r");
    if (reg_val & (1<<2))
      PRINTF("ERROR: SC16IS740 PARITY\n\r");
    if (reg_val & (1<<1))
      PRINTF("ERROR: SC16IS740 OVERRUN\n\r");

    if ((reg_val & (1<<0)) != 0) {
      rx_data[0] = SC16IS7X0_RHR;

      I2C_M_SETUP_Type transferMCfg;

      transferMCfg.sl_addr7bit = SC16IS740_ADDR;
      transferMCfg.retransmissions_max = 1;

      transferMCfg.tx_data = rx_data;
      transferMCfg.tx_length = 1;
      transferMCfg.rx_data = rx_data;
      transferMCfg.rx_length = 1;

      I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

      return(rx_data[0]);
    }
  }

  return(-1);
}

void sc16is7x0_write_data(uint8_t length, uint8_t *data) {
  for (uint8_t i=0;i<length;i++)
    sc16is7x0_write_char(data[i]);
}

void sc16is7x0_reset_rx_fifo(void) {
  sc16is7x0_set_reg(SC16IS7X0_FCR, (1<<1));
  sc16is7x0_set_reg(SC16IS7X0_FCR, 0x01);
}

void sc16is7x0_reset_tx_fifo(void) {
  sc16is7x0_set_reg(SC16IS7X0_FCR, (1<<2));
  sc16is7x0_set_reg(SC16IS7X0_FCR, 0x01);
}

void sc16is7x0_1ms_tick(void) {
  ms_counter++;
}
