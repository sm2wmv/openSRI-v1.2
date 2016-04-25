#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "LPC17xx.h"
#include "lpc_types.h"
#include "rtty.h"

#define RTTY_FIGS     27
#define RTTY_LETTERS  31

const struct_baudot baudot_table[91] = {
    {0,0},  //NULL
    {0,0},  //SOH
    {0,0},  //STX
    {0,0},  //ETX
    {0,0},  //EOT
    {0,0},  //ENQ
    {0,0},  //ACK
    {11,1},  //BEL
    {0,0},  //BS
    {0,0},  //TAB
    {2,0},  //LF
    {0,0},  //VT
    {0,0},  //FF
    {8,0},  //CR
    {0,0},  //S0
    {0,0},  //SI
    {0,0},  //DLE
    {0,0},  //DC1
    {0,0},  //DC2
    {0,0},  //DC3
    {0,0},  //DC4
    {0,0},  //NAK
    {0,0},  //SYN
    {0,0},  //ETB
    {0,0},  //CAN
    {0,0},  //EM
    {0,0},  //SUB
    {0,0},  //ESC
    {0,0},  //FS
    {0,0},  //GS
    {0,0},  //RS
    {0,0},  //US
    {4,0},  //Space
    {13,1},  //!
    {17,1},  //"
    {20,1},  //#
    {18,1},  //$
    {0,0},  //%
    {26,1},  //&
    {0,0},  //'
    {15,1},  //(
    {18,1},  //)
    {0,0},  //*
    {17,1},  //+
    {12,1},  //,
    {3,1},  //-
    {28,1},  //.
    {29,1},  ///
    {22,1},  //0
    {23,1},  //1
    {19,1},  //2
    {1,1},  //3
    {10,1},  //4
    {16,1},  //5
    {21,1},  //6
    {7,1},  //7
    {6,1},  //8
    {24,1},  //9
    {14,1},  //:
    {30,1},  //;
    {0,0},  //<
    {0,0},  //=
    {0,0},  //>
    {25,1},  //?
    {0,0},  //@
    {3,0},  //A
    {25,0},  //B
    {14,0},  //C
    {9,0},  //D
    {1,0},  //E
    {13,0},  //F
    {26,0},  //G
    {20,0},  //H
    {6,0},  //I
    {11,0},  //J
    {15,0},  //K
    {18,0},  //L
    {28,0},  //M
    {12,0},  //N
    {24,0},  //O
    {22,0},  //P
    {23,0},  //Q
    {10,0},  //R
    {5,0},  //S
    {16,0},  //T
    {7,0},  //U
    {30,0},  //V
    {19,0},  //W
    {29,0},  //X
    {21,0},  //Y
    {17,0},  //Z
};

uint8_t rtty_convert_str(int8_t *input_str, int8_t *return_str) {
  int8_t str[strlen((char *)input_str)*2];
  uint8_t pos=0;

  for (uint8_t i=0;i<strlen((char *)input_str);i++) {
    if (input_str[i] < 91) {
      if (baudot_table[input_str[i]].baudot_code != 0) {
        if (baudot_table[input_str[i]].figure_shift == 1) {
          if (i==0)
            str[pos++] = RTTY_FIGS;
          else if (baudot_table[input_str[i-1]].figure_shift != 1)
            str[pos++] = RTTY_FIGS;

          str[pos++] = baudot_table[input_str[i]].baudot_code;
        }
        else {
          if (i==0)
            str[pos++] = RTTY_LETTERS;
          else if (baudot_table[input_str[i-1]].figure_shift == 1)
            str[pos++] = RTTY_LETTERS;

          str[pos++] = baudot_table[input_str[i]].baudot_code;
        }
      }
    }
  }

  memcpy(return_str, str, pos);

  return(pos);
}
