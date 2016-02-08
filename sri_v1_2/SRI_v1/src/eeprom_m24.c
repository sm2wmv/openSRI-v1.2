#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LPC17xx.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"

#include "eeprom_m24.h"
#include "misc.h"
#include "main.h"
#include "event_queue.h"

static uint8_t write_in_progress = 0;

static int16_t counter_left = 0;
static uint8_t *data_ptr;
static uint16_t curr_addr = 0;

uint8_t eeprom_m24_write_byte(uint16_t eeprom_address, uint8_t value) {
  uint8_t data[3] = {(eeprom_address >> 8) & 0xFF, eeprom_address & 0xFF, value};

  PRINTF("EEPROM: WRITE BYTE ADDR: %i -  VAL: %i\n", eeprom_address, value);

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = EEPROM_M24_ADDR;
  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 3;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 1;

  return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t eeprom_m24_read_byte(uint16_t eeprom_address) {
  uint8_t ret[2];
  uint8_t data[2] = {(eeprom_address >> 8) & 0xFF, eeprom_address & 0xFF};

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = EEPROM_M24_ADDR;
  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = (uint8_t *)ret;
  transferMCfg.rx_length = 1;
  transferMCfg.retransmissions_max = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  return(ret[0]);
}

uint8_t eeprom_m24_write_block64(uint16_t start_address, uint8_t length, uint8_t *data) {
  uint8_t len = length;

  if (length > 64)
    len = 64;

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = EEPROM_M24_ADDR;
  transferMCfg.retransmissions_max = 1;

  uint8_t tx_data[64+2];

  tx_data[0] = (start_address >> 8) & 0xFF;
  tx_data[1] = start_address & 0xFF;

  for (uint8_t i=0;i<len;i++) {
    tx_data[i+2] = data[i];
//    PRINTF("OUT: %i\n",tx_data[i+2]);
  }

  transferMCfg.tx_data = (uint8_t *)&tx_data;
  transferMCfg.tx_length = len + 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;

  return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t eeprom_m24_read_block64(uint16_t start_address, uint8_t length, uint8_t *data) {
  uint8_t len = length;

  if (length > 64)
    len = 64;

  uint8_t tx_data[2] = {(start_address >> 8) & 0xFF, start_address & 0xFF};

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = EEPROM_M24_ADDR;
  transferMCfg.retransmissions_max = 1;

  transferMCfg.tx_data = (uint8_t *)&tx_data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = (uint8_t *)data;
  transferMCfg.rx_length = len;

  return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t eeprom_m24_write_block(uint16_t start_address, uint16_t length, uint8_t *data) {
  uint8_t retval = 0;
  uint16_t read_len = 0;

  PRINTF("EEPROM: WRITING BLOCK @%i - LEN: %i\n",start_address, length);

  if (write_in_progress == 0) {
    counter_left = length;
    data_ptr = data;
    curr_addr = start_address;
  }

  write_in_progress = 1;

  read_len = counter_left;

  if (counter_left > 64)
    read_len = 64;

  retval = eeprom_m24_write_block64(curr_addr, read_len, data_ptr);

  data_ptr += read_len;
  curr_addr += read_len;

  counter_left -= read_len;

  //Check if we have anymore to send. If that is the case we will add to run this
  //function again from the event queue with a 10 ms delay. That way we don't lock up the
  //system while saving data to the EEPROM
  if (counter_left <= 0) {
    write_in_progress = 0;
    main_eeprom_write_done();
  }
  else
    event_add_message((void *)main_eeprom_write_in_progress, 10, EEPROM_EVENT_TYPE_WRITE, EVENT_PRIORITY_LEVEL_MAINLOOP);

  return(retval);
}

uint8_t eeprom_m24_read_block(uint16_t start_address, uint16_t length, uint8_t *data) {
  uint8_t retval = 0;
  uint16_t read_len = 0;

  PRINTF("EEPROM: READ BLOCK\n");

  if (write_in_progress == 0) {
    counter_left = length;
    data_ptr = data;
    curr_addr = start_address;

    while (counter_left > 0) {
      read_len = counter_left;

      if (counter_left > 64)
        read_len = 64;

      retval = eeprom_m24_read_block64(curr_addr, read_len, data_ptr);

      data_ptr += read_len;
      curr_addr += read_len;

      counter_left -= read_len;

      delay_10us(100);
    }
  }
  else {
    PRINTF("WRITE IN PROGRESS\n");
    retval = 0;
  }

  return(retval);
}

uint8_t eeprom_m24_get_write_in_progress(void) {
  return(write_in_progress);
}
