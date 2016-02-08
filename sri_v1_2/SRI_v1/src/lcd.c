#include "lcd.h"
#include "main.h"

void lcd_set_io_dir(void) {
  LCD_CTRL_DIR |= ( LCDEN | LCDRS | LCDRW );
  LCD_CTRL_CLR |= ( LCDEN | LCDRS | LCDRW );
  LCD_DATA_DIR |= LCD_DATA_MASK;
}

void lcd_toggle_enable(void) {
  LCD_CTRL_SET = LCDEN;
  delay_10us(1);  //TODO: Make it shorter?
  LCD_CTRL_CLR = LCDEN;
}

void lcd_send_command(uint8_t command) {
  LCD_CTRL_CLR = LCDRS;
  LCD_CTRL_SET = LCDEN;
  LCD_DATA_CLR = LCD_DATA_MASK;
  LCD_DATA_SET = command;

  lcd_toggle_enable();
}

void lcd_send_data(uint8_t data) {
  LCD_CTRL_SET = LCDEN;
  LCD_CTRL_SET = LCDRS;

  LCD_DATA_CLR = LCD_DATA_MASK;
  LCD_DATA_SET = data;

  lcd_toggle_enable();
  lcd_wait();
}

uint8_t lcd_goto_xy( unsigned int x, unsigned int y) {
  int retval = 0;

  if((x > 1) && (y > 15)) {
    retval = -1;
  }
  else {
    if(x == 0) {
      lcd_send_command( 0x80 + y );
      lcd_wait();
    }
    else if(x==1) {
      lcd_send_command( 0xC0 + y );
      lcd_wait();
    }
  }

  return retval;
}

void lcd_put_char(uint8_t data) {
  lcd_send_data(data);
}

void lcd_put_string(uint8_t line, uint8_t *str) {
  unsigned char len = MAX_CHAR_IN_ONE_LINE;

  lcd_goto_xy(line, 0);

  while(*str != '\0' && len--) {
    lcd_put_char(*str);
    str++;
  }

  //We always wish to write the full line,
  //to make sure we clear the characters not used
  while(len--) {
    lcd_put_char(' ');
  }
}

void lcd_wait(void) {
  LCD_DATA_DIR &= ~LCD_DATA_MASK;
  LCD_CTRL_CLR = LCDRS;
  LCD_CTRL_SET = LCDRW | LCDEN;

  while(LCD_CTRL_PIN & LCD_BUSY_FLAG); /* wait for busy flag to become low */

  LCD_CTRL_CLR = LCDEN | LCDRW;
  LCD_DATA_DIR |= LCD_DATA_MASK;
  delay_ms(1);
}

void lcd_clear(void) {
  lcd_send_command(0x01);
  delay_10us(10);
}

void lcd_init(void) {
  delay_ms(200);

  lcd_set_io_dir();
  LCD_CTRL_CLR = LCDEN;

  delay_10us(10);
  lcd_send_command(0x38);
  delay_ms(10);
  lcd_send_command(0x38);
  delay_ms(10);
  lcd_send_command(0x38);
  lcd_wait();
  lcd_send_command(0x38);
  lcd_wait();
  lcd_send_command(0x10);
  lcd_wait();
  lcd_send_command(0x06);
  lcd_wait();
  lcd_send_command(0x0C);
  lcd_wait();
}
