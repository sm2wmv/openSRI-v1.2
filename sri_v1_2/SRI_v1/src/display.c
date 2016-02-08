#include <stdio.h>
#include <string.h>

#include "lpc17xx.h"
#include "display.h"
#include "main.h"
#include "lcd.h"
#include "status.h"
#include "display.h"
#include "settings.h"
#include "winkey.h"
#include "misc.h"

static uint8_t display_ptt_state = 0;
static char lcd_chars[2][17];


void display_welcome_screen(void) {
  /*** IMPORTANT ***
   *   Out of respect of the creator, please do not change
   *   this initialization screen, thanks.
   *
   *   de SM2WMV (SJ2W) */

  lcd_put_string(LCD_LINE1,(uint8_t *)"  Smart  Radio  \0");
  lcd_put_string(LCD_LINE2,(uint8_t *)" Interface v1.0 \0");

  delay_ms(1500);

  lcd_clear();
  lcd_put_string(LCD_LINE1,(uint8_t *)"   Created by   \0");
  lcd_put_string(LCD_LINE2,(uint8_t *)" SM2WMV  (SJ2W) \0");

  delay_ms(1500);

  lcd_clear();
}

void display_update(void) {
  if (main_init_done()) {
    uint8_t display_id_line1 = 0, display_id_line2 = 0;

    strcpy((char *)lcd_chars[0],"                ");
    strcpy((char *)lcd_chars[1],"                ");

    //Transmitting
    if (display_ptt_state == 1) {
      display_id_line1 = settings_get_display_id(DISPLAY_LINE_ID_LINE1_TX,status_get_vfo_mode_type()-1);
      display_id_line2 = settings_get_display_id(DISPLAY_LINE_ID_LINE2_TX,status_get_vfo_mode_type()-1);

      if (display_id_line1 == DISPLAY_TYPE_ID_CALL)
        sprintf(lcd_chars[0], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE1_TX, status_get_vfo_mode_type()-1), (char *)settings_get_callsign());
      else if (display_id_line1 == DISPLAY_TYPE_ID_MODE) {
        char mode[10];
        misc_convert_mode((char *)mode, status_get_vfo_mode());
        sprintf(lcd_chars[0], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE1_TX, status_get_vfo_mode_type()-1), (char *)mode);
      }
      else if (display_id_line1 == DISPLAY_TYPE_ID_FREQ) {
        char freq[10];
        misc_convert_freq((char *)freq, status_get_vfo_freq());
        sprintf(lcd_chars[0], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE1_TX, status_get_vfo_mode_type()-1), (char *)freq);
      }
      else if (display_id_line1 == DISPLAY_TYPE_ID_CW_SPEED) {
        sprintf(lcd_chars[0], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE1_TX, status_get_vfo_mode_type()-1), status_get_winkey_pot_speed());
      }

      if (display_id_line2 == DISPLAY_TYPE_ID_CALL)
        sprintf(lcd_chars[1], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE2_TX, status_get_vfo_mode_type()-1), (char *)settings_get_callsign());
      else if (display_id_line2 == DISPLAY_TYPE_ID_MODE) {
        char mode[10];
        misc_convert_mode((char *)mode, status_get_vfo_mode());
        sprintf(lcd_chars[1], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE2_TX, status_get_vfo_mode_type()-1), (char *)mode);
      }
      else if (display_id_line2 == DISPLAY_TYPE_ID_FREQ) {
        char freq[10];
        misc_convert_freq((char *)freq, status_get_vfo_freq());
        sprintf(lcd_chars[1], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE2_TX, status_get_vfo_mode_type()-1), (char *)freq);
      }
      else if (display_id_line2 == DISPLAY_TYPE_ID_CW_SPEED) {
        sprintf(lcd_chars[1], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE2_TX, status_get_vfo_mode_type()-1), status_get_winkey_pot_speed());
      }
    }
    else {
      display_id_line1 = settings_get_display_id(DISPLAY_LINE_ID_LINE1_RX,status_get_vfo_mode_type()-1);
      display_id_line2 = settings_get_display_id(DISPLAY_LINE_ID_LINE2_RX,status_get_vfo_mode_type()-1);

      if (display_id_line1 == DISPLAY_TYPE_ID_CALL)
        sprintf(lcd_chars[0], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE1_RX, status_get_vfo_mode_type()-1), (char *)settings_get_callsign());
      else if (display_id_line1 == DISPLAY_TYPE_ID_MODE) {
        char mode[10];
        misc_convert_mode((char *)mode, status_get_vfo_mode());
        sprintf(lcd_chars[0], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE1_RX, status_get_vfo_mode_type()-1), (char *)mode);
      }
      else if (display_id_line1 == DISPLAY_TYPE_ID_FREQ) {
        char freq[10];
        misc_convert_freq((char *)freq, status_get_vfo_freq());
        sprintf(lcd_chars[0], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE1_RX, status_get_vfo_mode_type()-1), (char *)freq);
      }
      else if (display_id_line1 == DISPLAY_TYPE_ID_CW_SPEED) {
        sprintf(lcd_chars[0], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE1_RX, status_get_vfo_mode_type()-1), status_get_winkey_pot_speed());
      }

      if (display_id_line2 == DISPLAY_TYPE_ID_CALL)
        sprintf(lcd_chars[1], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE2_RX, status_get_vfo_mode_type()-1), (char *)settings_get_callsign());
      else if (display_id_line2 == DISPLAY_TYPE_ID_MODE) {
        char mode[10];
        misc_convert_mode((char *)mode, status_get_vfo_mode());
        sprintf(lcd_chars[1], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE2_RX, status_get_vfo_mode_type()-1), (char *)mode);
      }
      else if (display_id_line2 == DISPLAY_TYPE_ID_FREQ) {
        char freq[10];
        misc_convert_freq((char *)freq, status_get_vfo_freq());
        sprintf(lcd_chars[1], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE2_RX, status_get_vfo_mode_type()-1), (char *)freq);
      }
      else if (display_id_line2 == DISPLAY_TYPE_ID_CW_SPEED) {
        sprintf(lcd_chars[1], (char *)settings_get_display_text(DISPLAY_LINE_ID_LINE2_RX, status_get_vfo_mode_type()-1), status_get_winkey_pot_speed());
      }
    }

    //lcd_clear();
    lcd_put_string(LCD_LINE1, (uint8_t *)lcd_chars[0]);
    lcd_put_string(LCD_LINE2, (uint8_t *)lcd_chars[1]);
  }
}

void display_ptt_active(void) {
  display_ptt_state = 1;

  display_update();
}

void display_ptt_deactive(void) {
  display_ptt_state = 0;

  display_update();
}
