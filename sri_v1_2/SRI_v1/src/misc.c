#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "misc.h"
#include "status.h"
#include "lpc17xx_uart.h"
#include "commands.h"
#include "comm_interface.h"
#include "main.h"

void PRINTF_TX(const char *format, ...);
void PRINTF_RAW_TX(const char *format, ...);

static uint8_t debug_state = 0;

static uint8_t system_debug_state = 0;

static uint8_t winkey_debug_state = 0;
static uint8_t cat_debug_state = 0;
static uint8_t sequencer_debug_state = 0;

void misc_set_debug_state(uint8_t state) {
  debug_state = state;
}

void misc_set_system_debug_state(uint8_t state) {
  system_debug_state = state;
}

void misc_set_winkey_debug_state(uint8_t state) {
  winkey_debug_state = state;
}

void misc_set_cat_debug_state(uint8_t state) {
  cat_debug_state = state;
}

void misc_set_sequencer_debug_state(uint8_t state) {
  sequencer_debug_state = state;
}

void PRINTF_CAT(const char *format, ...) {
  if (cat_debug_state != 0)
    PRINTF_TX(format);

  #ifdef WK_PORT_DEBUG
    PRINTF_RAW_TX(format);
  #endif

  #ifdef CAT_PORT_DEBUG
    PRINTF_RAW_TX(format);
  #endif
}

void PRINTF_SEQ(const char *format, ...) {
  if (sequencer_debug_state != 0)
    PRINTF_TX(format);
}

void PRINTF_WK(const char *format, ...) {
  if (winkey_debug_state != 0)
    PRINTF_TX(format);

  #ifdef WK_PORT_DEBUG
    PRINTF_RAW_TX(format);
  #endif

  #ifdef CAT_PORT_DEBUG
    PRINTF_RAW_TX(format);
  #endif
}

void PRINTF(const char *format, ...) {
  if (system_debug_state != 0)
    PRINTF_TX(format);

  #ifdef WK_PORT_DEBUG
    PRINTF_RAW_TX(format);
  #endif

  #ifdef CAT_PORT_DEBUG
    PRINTF_RAW_TX(format);
  #endif
}

void PRINTF_TX(const char *format, ...) {
  if (debug_state == 1) {
    char debug_output_str[DEBUG_STR_LEN];

    uint8_t retlen = sprintf(debug_output_str, "%010u: ",main_get_timer0_counter());

    va_list debug_arg;
    va_start (debug_arg, format);
    int debug_res = vsnprintf(debug_output_str+retlen,sizeof(debug_output_str),format,debug_arg);
    va_end(debug_arg);

    if (debug_res > 0)
      comm_interface_add_tx_message(SRI_CMD_DEBUG_TEXT, debug_res+retlen, (uint8_t *)debug_output_str);
  }
}


void PRINTF_RAW_TX(const char *format, ...) {
  #ifdef WK_PORT_DEBUG
    char output_str[DEBUG_STR_LEN];

    va_list arg;
    va_start (arg, format);
    int res = vsnprintf(output_str,sizeof(output_str),format,arg);
    va_end(arg);

    if (res > 0)
      UART_Send(LPC_UART3, (uint8_t *)output_str, res, BLOCKING);
  #endif

  #ifdef CAT_PORT_DEBUG
    char output_str[DEBUG_STR_LEN];

    va_list arg;
    va_start (arg, format);
    int res = vsnprintf(output_str,sizeof(output_str),format,arg);
    va_end(arg);

    if (res > 0)
      UART_Send(LPC_UART2, (char *)output_str, res, BLOCKING);
  #endif
}

void misc_convert_freq(char *freq, uint32_t freq_int) {
  sprintf((char *)freq, "%10d", freq_int);
  char temp_freq[10];

  temp_freq[9] = freq[8];
  temp_freq[8] = freq[7];
  temp_freq[7] = '.';
  temp_freq[6] = freq[6];
  temp_freq[5] = freq[5];
  temp_freq[4] = freq[4];

  if (freq_int >= 1000000)
    temp_freq[3] = '.';
  else
    temp_freq[3] = ' ';

  temp_freq[2] = freq[3];
  temp_freq[1] = freq[2];
  temp_freq[0] = freq[1];

  uint8_t temp_cnt = 0;
  for (uint8_t i=0;i<10;i++) {
    if (temp_freq[i] != ' ')
      freq[temp_cnt++] = temp_freq[i];
  }

  freq[temp_cnt] = 0;

  //strcpy((char *)freq, (char *)temp_freq);
}

void misc_convert_mode(char *mode, uint8_t mode_int) {
  if (mode_int == STATUS_RADIO_MODE_LSB)
    strcpy(mode, "LSB");
  else if (mode_int == STATUS_RADIO_MODE_USB)
    strcpy(mode, "USB");
  else if (mode_int == STATUS_RADIO_MODE_CW)
    strcpy(mode, "CW");
  else if (mode_int == STATUS_RADIO_MODE_FM)
    strcpy(mode, "FM");
  else if (mode_int == STATUS_RADIO_MODE_AM)
    strcpy(mode, "AM");
  else if (mode_int == STATUS_RADIO_MODE_FSK)
    strcpy(mode, "FSK");
  else if (mode_int == STATUS_RADIO_MODE_CWR)
    strcpy(mode, "CW-R");
  else if (mode_int == STATUS_RADIO_MODE_PKTL)
    strcpy(mode, "PKT-L");
  else if (mode_int == STATUS_RADIO_MODE_FSKR)
    strcpy(mode, "FSK-R");
  else if (mode_int == STATUS_RADIO_MODE_PKTFM)
    strcpy(mode, "PKT-FM");
  else if (mode_int == STATUS_RADIO_MODE_FMN)
    strcpy(mode, "FM-N");
  else if (mode_int == STATUS_RADIO_MODE_PKTU)
    strcpy(mode, "PKT-U");
  else
    strcpy(mode, "UNKNOWN");
}
