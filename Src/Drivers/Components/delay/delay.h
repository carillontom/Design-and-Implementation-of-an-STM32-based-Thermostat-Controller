/*
 * delay.h
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */

#ifndef DRIVERS_COMPONENTS_DELAY_DELAY_H_
#define DRIVERS_COMPONENTS_DELAY_DELAY_H_

#include <stdint.h>

void delay_init(uint32_t system_core_clock_hz);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#endif /* DRIVERS_COMPONENTS_DELAY_DELAY_H_ */
