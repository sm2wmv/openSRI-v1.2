#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>

#define FLAG_RUN_EVENT_QUEUE  0

//! The interval between each PING is sent from both computer and SRI interface
#define SRI_PING_INTERVAL     1000

//! The timeout limit to know we have lost connection to the computer
#define SRI_PING_TIMEOUT_LIMIT     5000

typedef struct {
  uint8_t various;
  uint8_t init_done;
} struct_flags;

/* Byte which is read from the EEPROM to check if the proper settings have
   been made. This can be changed to force loading default settings because
   of major change in data structures */
#define SYSTEM_INIT_BYTE_VAL  8

#define BUTTON_ENCODER_INDEX  0
#define BUTTON1_INDEX         1
#define BUTTON2_INDEX         2
#define BUTTON3_INDEX         3

uint8_t main_init_done(void);

void main_eeprom_write_in_progress(void);
void main_eeprom_write_done(void);

void event_add_message(void (*func), uint16_t offset, uint8_t id, uint8_t priority);

uint32_t main_get_timer0_counter(void);

void main_timer0_tick(void);
void main_timer1_tick(void);

void delay_ms(uint32_t time);
void delay_10us(uint32_t time);

#endif
