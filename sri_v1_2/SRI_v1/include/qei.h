#ifndef __QEI_H
#define __QEI_H

void qei_init_hw(void);

int16_t qei_get_counter_pos(void);
void qei_reset_counter_pos(void);
void qei_set_counter_pos(int16_t new_pos);
int16_t qei_get_counter_change();

#endif /* end __QEI_H */
/****************************************************************************
** End Of File
*****************************************************************************/
