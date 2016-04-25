#ifndef __BOARD_H
#define __BOARD_H

/* BOARD hardware v1.1
 * -------------------
 *
 * PORT 0
 * ------
 * P0_00 -> Output - Winkey TXD
 * P0_01 -> Input  - Winkey RXD
 * P0_02 -> Output - LPC1765 UART0 TXD
 * P0_03 -> Input  - LPC1765 UART0 RXD
 * P0_04 -> Input  - AUX IN
 * P0_05 -> Output - Radio Inhibit
 * P0_06 -> Output - SPI SSEL
 * P0_07 -> Output - SPI SCK
 * P0_08 -> Input  - SPI MISO
 * P0_09 -> Output - SPI MOSI
 * P0_10 -> Output - FT4232 CAT RXD
 * P0_11 -> Input  - FT4232 CAT TXD
 * P0_15 -> Output - CAT TXD
 * P0_16 -> Input  - CAT RXD
 * P0_17 -> Output - CAT RTS
 * P0_18 -> Input  - CAT CTS
 * P0_19 -> Output - Radio PTT1
 * P0_20 -> Output - Radio PTT2
 * P0_21 -> Output - Radio FSK
 * P0_22 -> Output - Radio CW
 * P0_23 -> Input  - SC16IS7X0 IRQ
 * P0_24 -> Input  - DC input voltage
 * P0_25 -> Input  - FLOOR AUX 6
 * P0_26 -> Output - TX Active
 * P0_27 -> Output - I2C SDA
 * P0_28 -> Output - I2C SCL
 * P0_29 -> I/O    - USB DP (Host)
 * P0_30 -> I/O    - USB DM (Host)
 *
 * PORT 1
 * ------
 * P1_00 -> Input  - FT4232 CAT RTS
 * P1_01 -> Output - FT4232 CAT CTS
 * P1_04 -> Input  - FLOOR AUX 0
 * P1_08 -> Input  - FLOOR AUX 1
 * P1_09 -> Input  - FLOOR AUX 2
 * P1_10 -> Input  - FLOOR AUX 3
 * P1_14 -> Input  - FLOOR AUX 4
 * P1_15 -> Input  - FLOOR AUX 5
 * P1_16 -> Input  - FT4232 WK RTS
 * P1_17 -> Input  - FT4232 WK DTR
 * P1_18 -> Output - Front LCD RS
 * P1_19 -> Output - USB PPWR
 * P1_20 -> Input  - Front ENCODER PHA
 * P1_21 -> Output - Front LCD RW
 * P1_22 -> Input  - USB PWRD
 * P1_23 -> Input  - Front ENCODER PHB
 * P1_24 -> Output - PTT info OUT1
 * P1_25 -> Output - PTT info OUT2
 * P1_26 -> Output - Front Backlight PWM
 * P1_27 -> Input  - USB OVRCR
 * P1_28 -> Output - Front LED CW
 * P1_29 -> Input  - Front LCD Enable
 * P1_30 -> Input  - Winkey PTT
 * P1_31 -> Input  - Winkey KEY
 *
 * PORT 2
 * ------
 * P2_00 -> Output - Front LCD DB0
 * P2_01 -> Output - Front LCD DB1
 * P2_02 -> Output - Front LCD DB2
 * P2_03 -> Output - Front LCD DB3
 * P2_04 -> Output - Front LCD DB4
 * P2_05 -> Output - Front LCD DB5
 * P2_06 -> Output - Front LCD DB6
 * P2_07 -> Output - Front LCD DB7
 * P2_08 -> Input  - Front EXP interrupt
 * P2_09 -> Input  - PTT IN
 * P2_10 -> Input  - Front Encoder Button
 * P2_11 -> Input  - FT4232 FSKCW CW
 * P2_12 -> Input  - FT4232 FSKCW FSK
 * P2_13 -> Input  - FT4232 FSKCW PTT
 *
 * PORT 3
 * ------
 * P3_25 -> Output - PTT AMP
 * P3_26 -> Input  - Footswitch
 *
 * PORT 4
 * ------
 * P4_28 -> Output - AUX OUT
 * P4_29 -> Output - Board LED Green
 */

/* I2C addresses of IO Expanders (8-bit addr)*/
#define IO_EXPA0_ADDR    0x40
#define IO_EXPA1_ADDR    0x42
#define IO_EXPA2_ADDR    0x44
#define IO_EXPA3_ADDR    0x46
#define IO_EXPA4_ADDR    0x48
#define IO_EXPA5_ADDR    0x4A
#define IO_EXPA6_ADDR    0x4C
#define IO_EXPA7_ADDR    0x4E
#define IO_EXPA8_ADDR    0x50

//! Address of the SC16IS740 chip (7-bit address)
#define SC16IS740_ADDR        0x4D

//! Address of the LP5521 chip (8-bit address)
#define LP5521_ADDR 0x64

/* PORT 0 */
#define AUX_IN					        4
#define AUX_IN_PORT				      LPC_GPIO0
#define RADIO_INHIBIT			      5
#define RADIO_INHIBIT_PORT		  LPC_GPIO0
#define SPI_SSEL				        6
#define SPI_SSEL_PORT			      LPC_GPIO0
#define LPC1765_CAT_RTS			    17
#define LPC1765_CAT_RTS_PORT    LPC_GPIO0
#define LPC1765_CAT_CTS			    18
#define LPC1765_CAT_CTS_PORT    LPC_GPIO0
#define RADIO_PTT_1             19
#define RADIO_PTT_1_PORT        LPC_GPIO0
#define RADIO_PTT_2             20
#define RADIO_PTT_2_PORT        LPC_GPIO0
#define RADIO_FSK               21
#define RADIO_FSK_PORT          LPC_GPIO0
#define RADIO_CW                22
#define RADIO_CW_PORT           LPC_GPIO0
#define SC16IS7X0_IRQ			      23
#define SC16IS7X0_IRQ_PORT		  LPC_GPIO0
#define EXT_VIN_MEAS			      24
#define EXT_VIN_MEAS_PORT		    LPC_GPIO0
#define FLOOR_AUX6	          	25
#define FLOOR_AUX6_PORT      	  LPC_GPIO0
#define TX_ACTIVE				        26
#define TX_ACTIVE_PORT			    LPC_GPIO0

/* PORT 1 */
#define FT4232_CAT_RTS			    0
#define FT4232_CAT_RTS_PORT		  LPC_GPIO1
#define FT4232_CAT_CTS			    1
#define FT4232_CAT_CTS_PORT		  LPC_GPIO1
#define FLOOR_AUX0              4  //ANT PIN
#define FLOOR_AUX0_PORT         LPC_GPIO1
#define FLOOR_AUX1            	8
#define FLOOR_AUX1_PORT         LPC_GPIO1
#define FLOOR_AUX2            	9
#define FLOOR_AUX2PORT          LPC_GPIO1
#define FLOOR_AUX3            	10
#define FLOOR_AUX3_PORT         LPC_GPIO1
#define FLOOR_AUX4            	14
#define FLOOR_AUX4_PORT         LPC_GPIO1
#define FLOOR_AUX5            	15
#define FLOOR_AUX5_PORT         LPC_GPIO1
#define FT4232_WK_RTS			      16
#define FT4232_WK_RTS_PORT		  LPC_GPIO1
#define FT4232_WK_DTR			      17
#define FT4232_WK_DTR_PORT		  LPC_GPIO1
#define LCD_RS 		              18
#define LCD_RS_PORT             LPC_GPIO1
#define FRONT_PHA				        19
#define FRONT_PHA_PORT			    LPC_GPIO1
#define FRONT_PHB				        23
#define FRONT_PHB_PORT			    LPC_GPIO1
#define PTT_INFO_OUT1			      24
#define PTT_INFO_OUT1_PORT		  LPC_GPIO1
#define PTT_INFO_OUT2			      25
#define PTT_INFO_OUT2_PORT		  LPC_GPIO1
#define FRONT_PWM_BACKLIGHT		  26
#define FRONT_PWM_BACKLIGHT_PORT	LPC_GPIO1
#define LCD_RW 		              21
#define LCD_RW_PORT             LPC_GPIO1
#define FRONT_LED_CW            28
#define FRONT_LED_CW_PORT       LPC_GPIO1
#define LCD_EN	                29
#define LCD_EN_PORT             LPC_GPIO1
#define WINKEY_PTT		          30
#define WINKEY_PTT_PORT         LPC_GPIO1
#define WINKEY_KEY              31
#define WINKEY_KEY_PORT         LPC_GPIO1


/* PORT 2 */
#define LCD_DB0                 0
#define LCD_DB1                 1
#define LCD_DB2                 2
#define LCD_DB3                 3
#define LCD_DB4                 4
#define LCD_DB5                 5
#define LCD_DB6                 6
#define LCD_DB7                 7
#define FRONT_EXP_INT			      8
#define FRONT_EXP_INT_PORT		  LPC_GPIO2
#define PTT_IN					        9
#define PTT_IN_PORT				      LPC_GPIO2
#define FRONT_ENCODER_BUTTON	  10
#define FRONT_ENCODER_BUTTON_PORT	LPC_GPIO2
#define FT4232_FSKCW_CW			    11
#define FT4232_FSKCW_CW_PORT	  LPC_GPIO2
#define FT4232_FSKCW_FSK		    12
#define FT4232_FSKCW_FSK_PORT	  LPC_GPIO2
#define FT4232_FSKCW_PTT		    13
#define FT4232_FSKCW_PTT_PORT	  LPC_GPIO2

/* PORT 3 */
#define PTT_AMP                 25
#define PTT_AMP_PORT            LPC_GPIO3
#define FOOTSW                  26
#define FOOTSW_PORT             LPC_GPIO3


/* PORT 4 */
#define BOARD_AUX_OUT			    28
#define BOARD_AUX_OUT_PORT		LPC_GPIO4
#define BOARD_LED_GREEN       29
#define BOARD_LED_GREEN_PORT  LPC_GPIO4

#define AUX_RELAY_K1  (1<<9)
#define AUX_RELAY_K2  (1<<10)
#define AUX_RELAY_K3  (1<<11)
#define AUX_RELAY_K4  (1<<12)
#define AUX_RELAY_K5  (1<<13)
#define AUX_RELAY_K6  (1<<1)
#define AUX_RELAY_K7  (1<<2)
#define AUX_RELAY_K8  (1<<3)
#define AUX_RELAY_K9  (1<<4)
#define AUX_RELAY_K10 (1<<5)

#endif /* end __BOARD_H */

/****************************************************************************
**                            End Of File
*****************************************************************************/
