#include <stdio.h>

#include "lpc17xx.h"
#include "qei.h"
#include "lcd.h"

static int16_t qei_counter = 0;
static int16_t qei_counter_prev_pos = 0;

void init_qei(void) {
  //Basic Configuration:
  LPC_SC->PCONP |= (1<<18);
  LPC_SC->PCLKSEL1  |= (0x01);

  LPC_QEI->QEICONF = 0;//PhA and PhB function as quadrature encoder inputs (2x)
  LPC_QEI->QEICON = 0x02;//Control register: Reset position counter on index.(RESPI)
  LPC_QEI->INXCMP = 0x00;//QEI Index Compare register                                                                                                           LPC_QEI->QEILOAD = 0x00;//QEI Timer Reload register
  LPC_QEI->QEICLR = 0xFFFFFFFF;
  LPC_QEI->QEIIEC = 0xFFFFFFFF;
  LPC_QEI->QEIIES = 0x00000020;//enable ENCLK_EN
  LPC_QEI->FILTER = 30000;

  NVIC_EnableIRQ(QEI_IRQn);               /*enable the interrupt*/
}

void QEI_IRQHandler(void){
  if (LPC_QEI->QEISTAT & 0x01)
    qei_counter--;
  else
    qei_counter++;

  LPC_QEI->QEICLR = 0xFFFFFFFF;   // clear all interrupts
}

int16_t qei_get_counter_pos(void) {
  return(qei_counter);
}

void qei_reset_counter_pos(void) {
  qei_counter = 0;
}

void qei_set_counter_pos(int16_t new_pos) {
  qei_counter = new_pos;
}

int16_t qei_get_counter_change() {
  int16_t temp = qei_counter - qei_counter_prev_pos;
  qei_counter_prev_pos = qei_counter;

  return(temp);
}
