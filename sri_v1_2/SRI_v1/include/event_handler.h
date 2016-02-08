#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include <stdio.h>
#include "setting_structs.h"
#include "comm_interface.h"
#include "commands.h"
#include "status.h"
#include "lpc17xx.h"

//! The interval between each time we poll the PTT inputs, this is in 10us, so 10 = 10*10 us
#define EVENT_HANDLER_POLL_PTT_STATUS_INTERVAL  10

#define EVENT_HANDLER_CHECK_PTT_INPUTS 0

typedef struct {
  uint32_t temp_ptt_input;
  uint8_t curr_ptt_input;
  uint8_t prev_ptt_input;
  uint8_t curr_cw_input;
  uint8_t prev_cw_input;
  uint8_t temp_cw_input;
  uint8_t cw_led_status;
  uint8_t flags;
} struct_event_status;

void event_handler_ptt_radio_activate(void);
void event_handler_ptt_radio_deactivate(void);
void event_handler_ptt_amp_activate(void);
void event_handler_ptt_amp_deactivate(void);
void event_handler_ptt_inhibit_activate(void);
void event_handler_ptt_inhibit_deactivate(void);
void event_handler_ptt_ant_activate(void);
void event_handler_ptt_ant_deactivate(void);

void event_handler_radio_status_changed(struct_radio_status *curr_status);
void event_handler_execute_audio_change(uint8_t ptt_state);
void event_handler_execute_ptt_change(uint8_t prev_state, uint8_t curr_state);
uint8_t event_handler_get_ptt_status(void);
void event_handler_execute(void);

void event_handler_10us_tick(void);
void event_handler_1ms_tick(void);

#endif
