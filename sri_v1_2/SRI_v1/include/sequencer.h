#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

#include <stdint.h>

//! Bit to enable PTT input via the Footswitch
#define PTT_INPUT_FOOTSWITCH    0
//! Bit to enable PTT input via the Winkey ports RTS
#define PTT_INPUT_WK_RTS        1
//! Bit to enable PTT input via the Winkey ports DTR
#define PTT_INPUT_WK_DTR        2
//! Bit to enable PTT input via the FSK/CW port RTS
#define PTT_INPUT_FSKCW_RTS     3
//! Bit to enable PTT input via VOX sense input
#define PTT_INPUT_VOX_SENSE     4
//! Bit to enable PTT input via WinKey input
#define PTT_INPUT_WK            5

uint8_t sequencer_get_ptt_active(void);

void sequencer_activate(void);
void sequencer_deactivate(void);

void sequencer_lock_all(void);

#endif
