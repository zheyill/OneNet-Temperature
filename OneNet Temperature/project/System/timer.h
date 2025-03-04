#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

void timer2_init(uint16_t period, uint16_t prescaler);
void time1_pwm_init(uint16_t period, uint16_t prescaler);

#endif
