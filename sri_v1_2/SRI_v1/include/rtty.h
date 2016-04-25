#ifndef _RTTY_H_
#define _RTTY_H_

typedef struct {
  uint8_t baudot_code;
  uint8_t figure_shift;
} struct_baudot;

uint8_t rtty_convert_str(int8_t *input_str, int8_t *return_str);

#endif
