/*
 * delay.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */

#include "delay.h"
#include "systick.h"
#include "stm32f4xx.h"

static uint32_t gu32CpuFreqHz = 0U;
static uint32_t gu32CyclesPerUs = 0U;

void delay_init(uint32_t system_core_clock_hz)
{
    gu32CpuFreqHz = system_core_clock_hz;
    gu32CyclesPerUs = gu32CpuFreqHz / 1000000U;

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = systick_get_tick();

    while ((systick_get_tick() - start) < ms)
    {
    }
}

void delay_us(uint32_t us)
{
    uint32_t start;
    uint32_t cycles;

    if (gu32CyclesPerUs == 0U)
    {
        return;
    }

    cycles = us * gu32CyclesPerUs;
    start = DWT->CYCCNT;

    while ((DWT->CYCCNT - start) < cycles)
    {
    }
}
