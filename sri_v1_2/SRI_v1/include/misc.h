#ifndef __MISC_H
#define __MISC_H

#include "lpc_types.h"

//#define WK_PORT_DEBUG
//#define CAT_PORT_DEBUG

#define DEBUG_STR_LEN 80

void PRINTF(const char *format, ...);
void PRINTF_WK(const char *format, ...);
void PRINTF_CAT(const char *format, ...);
void PRINTF_SEQ(const char *format, ...);

void misc_convert_freq(char *freq, uint32_t freq_int);
void misc_convert_mode(char *mode, uint8_t mode_int);

void misc_set_debug_state(uint8_t state);
void misc_set_system_debug_state(uint8_t state);
void misc_set_winkey_debug_state(uint8_t state);
void misc_set_cat_debug_state(uint8_t state);
void misc_set_sequencer_debug_state(uint8_t state);

#endif
