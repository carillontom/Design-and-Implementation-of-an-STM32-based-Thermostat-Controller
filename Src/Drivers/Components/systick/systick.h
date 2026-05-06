/*
 * systick.h
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */

#ifndef COMPONENTS_SYSTICK_SYSTICK_H_
#define COMPONENTS_SYSTICK_SYSTICK_H_

#include <stdint.h>

void systick_init(uint32_t system_core_clock_hz);
uint32_t systick_get_tick(void);
void systick_delay_ms(uint32_t ms);
void systick_irq_handler(void);

#endif /* COMPONENTS_SYSTICK_SYSTICK_H_ */
