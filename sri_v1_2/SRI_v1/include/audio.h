#ifndef __AUDIO_H
#define __AUDIO_H

void audio_init(void);
void audio_set_state(uint8_t mode, uint8_t route_index);
void audio_radio_afin_source(uint8_t source);
void audio_set_mic_mute_state(uint8_t type);
void audio_set_line_out_mute_state(uint8_t type);
void audio_set_mic_preamp_state(uint8_t type);
void audio_set_mic_ext_preamp_state(uint8_t type);
void audio_set_mic_path(uint8_t type);

#endif
