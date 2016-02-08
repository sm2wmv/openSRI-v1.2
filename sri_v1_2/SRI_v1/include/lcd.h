#ifndef _LCD_H
#define _LCD_H

#include "lpc17xx.h"
#include "board.h"

#define LCD_LINE1 0
#define LCD_LINE2 1

#define MAX_CHAR_IN_ONE_LINE 16

#define LCD_DATA_DIR	LPC_GPIO2->FIODIR
#define LCD_DATA_SET	LPC_GPIO2->FIOSET
#define LCD_DATA_CLR	LPC_GPIO2->FIOCLR

#define LCD_CTRL_DIR	  LPC_GPIO1->FIODIR
#define LCD_CTRL_SET    LPC_GPIO1->FIOSET
#define LCD_CTRL_CLR    LPC_GPIO1->FIOCLR
#define LCD_CTRL_PIN    LPC_GPIO1->FIOPIN

#define LCDRS			(1<<LCD_RS)
#define LCDRW			(1<<LCD_RW)
#define LCDEN			(1<<LCD_EN)


#define LCD_BUSY_FLAG	(1<<LCD_DB7)

#define LCD_DATA_MASK 0xFF

uint8_t lcd_goto_xy( unsigned int x, unsigned int y);
void lcd_init(void);
void lcd_clear(void);
void lcd_toggle_enable(void);
void lcd_send_data(uint8_t data);
void lcd_put_string(uint8_t line, uint8_t *str);
void lcd_put_char(uint8_t data);
void lcd_wait(void);

#endif
