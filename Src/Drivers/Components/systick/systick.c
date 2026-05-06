/*
 * systick.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */


#include "systick.h"
#include "stm32f4xx.h"

static volatile uint32_t gu32TickMs = 0;

void systick_init(uint32_t system_core_clock_hz)
{
    SysTick->LOAD = (system_core_clock_hz / 1000U) - 1U; // 16Mhz oscillator
    SysTick->VAL  = 0U;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
}

uint32_t systick_get_tick(void)
{
    return gu32TickMs;
}

void systick_delay_ms(uint32_t ms)
{
    uint32_t start = systick_get_tick();
    while ((systick_get_tick() - start) < ms)
    {
    }
}

void systick_irq_handler(void)
{
    gu32TickMs++;
}
