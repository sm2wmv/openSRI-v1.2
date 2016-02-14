#include <cr_section_macros.h>
#include <stdio.h>

#include <string.h>
#include <stdarg.h>

#include "LPC17xx.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_uart.h"
#include "main.h"
#include "lpc_types.h"
#include "init.h"
#include "board.h"
#include "timer.h"
#include "ctrl.h"
#include "cat_interface.h"
#include "lcd.h"
#include "qei.h"
#include "settings.h"
#include "cw_ctrl.h"
#include "comm_interface.h"
#include "computer_interface.h"
#include "commands.h"
#include "version.h"
#include "misc.h"
#include "event_queue.h"
#include "eeprom_m24.h"
#include "event_handler.h"
#include "audio.h"
#include "sc16is7x0.h"
#include "winkey.h"
#include "display.h"

volatile uint32_t timer0_counter = 0;
volatile uint32_t timer1_counter = 0;
volatile uint32_t ms_delay_counter = 0;
volatile uint32_t us10_delay_counter = 0;

static uint16_t counter_event_timer = 0;

//static uint8_t lcd_data_line1[16];
//static uint8_t lcd_data_line2[16];

volatile struct_flags flags;

volatile uint8_t flag_read_buttons = 0;

void delay_ms(uint32_t time) {
  ms_delay_counter = 0;

  while(ms_delay_counter < time);
}

void delay_10us(uint32_t time) {
  us10_delay_counter = 0;

  while (us10_delay_counter < time);
}

void main_eeprom_write_in_progress(void) {
  eeprom_m24_write_block(0,0,NULL);
}


void main_eeprom_write_done(void) {
  PRINTF("EEPROM: WRITE DONE\n");

  //Send the message back to the software telling the user that
  //the EEPROM write has been done
  if (event_queue_check_id(EEPROM_EVENT_TYPE_WRITE) == 0)
    comm_interface_add_tx_message(SRI_CMD_EEPROM_SAVE,0,0);
}

void main_init_default_structures(void) {
  settings_set_default_settings();

  settings_save_to_eeprom();
  settings_save_cw_messages_to_eeprom();
}

/*! Add a message to the event queue which will be run at the correct time
 *  \param func A function pointer to the function we want to run
 *  \param offset the time in ms when we want our function to be run
 *  \param id Which type of event this is
 *  \param priority The priority type */
void event_add_message(void (*func), uint16_t offset, uint8_t id, uint8_t priority) {
  EVENT_MESSAGE event;

  if (event_in_queue() == 0)
    counter_event_timer = 0;

  event.func = func;
  event.time_target = counter_event_timer + offset;
  event.id = id;
  event.priority = priority;

  if (event_queue_add(event) == -1)
    PRINTF("EVENT QUEUE IS FULL\n");

  //PRINTF("ADDED TO QUEUE\n");
}

/*! \brief Run the first function in the event queue */
void event_run(void) {
  if (event_in_queue()) {
    EVENT_MESSAGE event = event_queue_get();

    //Drop the message from the event QUEUE
    event_queue_drop();

    //Run the function in the event queue
    event.func();
  }
}

uint32_t main_get_timer0_counter(void) {
  return(timer0_counter);
}

uint8_t main_init_done(void) {
  return(flags.init_done);
}


/*******************************************************************************
**   Main Function  main()
*******************************************************************************/
int main (void) {
  SystemCoreClockUpdate();

  //Set flag so we can know when the init
  //process has been finished
  flags.init_done = 0;

  //Initialize all I/Os
  init_io();

  //Init the I2C bus running 400kHz
  I2C_Init(LPC_I2C0,400000);
  I2C_Cmd(LPC_I2C0, ENABLE);

  //UART for the control software
  init_uart0(250000);

  #ifdef WK_PORT_DEBUG
    init_uart3(115200);
  #else
    init_uart3(1200); //Init to 1200 baud on the WinKey port
  #endif

  #ifdef CAT_PORT_DEBUG
    cat_interface_init_computer_uart(115200,0,0,0,0);
  #endif

  //Initialize and enable timer 0 to run with 1ms intervals
  init_timer(0 , 0);
  init_timer(1 , 0);

  enable_timer(0);
  enable_timer(1);

  comm_interface_init(computer_interface_uart_rx, computer_interface_uart_tx, computer_interface_uart_poll_rx);

  //Turn the power LED on
  ctrl_led_pwr_on();
  ctrl_led_cw_off();

  //Initialize stuff to their default states
  ctrl_mic_relay_set();
  ctrl_ptt_amp_clr();
  ctrl_pcm2912_mic_preamp_off();
  ctrl_pcm2912_mic_mute_off();
  ctrl_radio_input_source_low();
  ctrl_mic_ext_preamp_relay_clr();

  ctrl_i2c_io_exp_a0_set();
  ctrl_i2c_io_exp_a1_set();
  ctrl_i2c_io_exp_a2_set();
  ctrl_i2c_io_exp_a3_set();

  ctrl_fsk_tx_enable_clr();

  delay_ms(100);

  init_qei();

  lcd_init();

  event_queue_init();

  //Check that the box has been configured, if not we load the default settings
  if (eeprom_m24_read_byte(10) != SYSTEM_INIT_BYTE_VAL) {
    PRINTF("BYTE: %i\n",eeprom_m24_read_byte(10));
    PRINTF("DEFAULT INIT STRUCTURES\n");
    main_init_default_structures();

    PRINTF("WRITING INIT BYTE TO EEPROM\n");
    eeprom_m24_write_byte(10,SYSTEM_INIT_BYTE_VAL);
    PRINTF("INIT BYTE WRITTEN: %i\n",SYSTEM_INIT_BYTE_VAL);
  }
  else
    settings_load_from_eeprom(); //Load the settings from the EEPROM

  ctrl_backlight_init(settings_get_backlight_rgb_red(), settings_get_backlight_rgb_green(), settings_get_backlight_rgb_blue());

  #ifndef CAT_PORT_DEBUG
    cat_interface_init(settings_get_cat_baudrate(), settings_get_cat_stopbits(), settings_get_cat_parity(), settings_get_cat_flow_control(), settings_get_cat_jumper_cts_rts());
  #endif


  ctrl_backlight_init(settings_get_backlight_rgb_red(), settings_get_backlight_rgb_green(), settings_get_backlight_rgb_blue());

  sc16is7x0_init_winkey();

  //status.curr_button_state = ctrl_buttons_get_status();
  flags.various = 0;

  status_set_vfoA_mode(STATUS_RADIO_MODE_CW);
  status_set_vfoB_mode(STATUS_RADIO_MODE_CW);

  delay_ms(100);
  winkey_init();
  winkey_reset();

  display_welcome_screen();

  int16_t qei_temp;

  PRINTF("SRI interface started\n\r");

  flags.init_done = 1;

  display_update();

  uint8_t prev_radio_state = 255;

  while ( 1 ) {
    qei_temp = qei_get_counter_change();

    if (ctrl_get_radio_state() != prev_radio_state) {
      if (prev_radio_state == 0)
        PRINTF("RADIO IS ON\n");
      else
        PRINTF("RADIO IS OFF\n");

      prev_radio_state = ctrl_get_radio_state();
    }

    if (qei_temp != 0) {
      if (qei_temp > 0)
        winkey_inc_speed();
      else
        winkey_dec_speed();
    }

    comm_interface_parse_rx_buffer();
    comm_interface_poll_rx_queue();
    comm_interface_poll_tx_queue();

    event_handler_execute();

    #ifndef CAT_PORT_DEBUG
      cat_interface_execute();
    #endif

    computer_interface_execute();
    winkey_execute();

    //Read the button states
    if (flag_read_buttons != 0) {
      //status.curr_button_state = ctrl_buttons_get_status();
      flag_read_buttons = 0;
    }

    if (flags.various & (1<<FLAG_RUN_EVENT_QUEUE)) {
      event_run();
      flags.various &= ~(1<<FLAG_RUN_EVENT_QUEUE);
    }
  }
}

/*! \brief Gets called with 1 ms intervals */
void main_timer0_tick(void) {
  if (event_in_queue()) {
    if (counter_event_timer >= (event_queue_get()).time_target) {
      if (event_queue_get().priority == EVENT_PRIORITY_LEVEL_MAINLOOP)
        flags.various |= (1<<FLAG_RUN_EVENT_QUEUE);
      else if (event_queue_get().priority == EVENT_PRIORITY_LEVEL_REALTIME) {
        event_run();
      }
    }
  }

  //If the value equals the half of it's range then
  //we remove that amount from all target times in the event
  //queue and set the counter to 0 again
  if (counter_event_timer >= 32767) {
    event_queue_wrap(32767);

    counter_event_timer = 0;
  }

  //Blink the green LED on the board for debug purposes
  if ((timer0_counter % 500) == 0) {
    if (BOARD_LED_GREEN_PORT->FIOPIN & (1<<BOARD_LED_GREEN)) {
      BOARD_LED_GREEN_PORT->FIOCLR = (1<<BOARD_LED_GREEN);
    }
    else {
      BOARD_LED_GREEN_PORT->FIOSET = (1<<BOARD_LED_GREEN);
    }
  }

  if ((timer0_counter % 1000) == 0) {
    flag_read_buttons = 1;
  }

  //sc16is7x0_1ms_tick();

  if (flags.init_done) {
    comm_interface_1ms_tick();
    computer_interface_1ms_tick();
    cat_interface_1ms_tick();
    winkey_1ms_tick();
    event_handler_1ms_tick();
  }

  counter_event_timer++;
  ms_delay_counter++;
  timer0_counter++;
}

/*! \brief Gets called with 10 us intervals */
void main_timer1_tick(void) {
  if (flags.init_done) {
    event_handler_10us_tick();
  }

  timer1_counter++;
  us10_delay_counter++;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
