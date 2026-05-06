/*
 * bsp_button.h
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */

#ifndef BSP_BSP_BUTTON_BSP_BUTTON_H_
#define BSP_BSP_BUTTON_BSP_BUTTON_H_

#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    BUTTON_ACTIVE_LOW = 0,
    BUTTON_ACTIVE_HIGH
} button_active_t;

typedef struct
{
    GPIO_TypeDef    *port;
    uint8_t          pin;               /* pin number: 0..15 */
    button_active_t  active_level;

    bool             stable_state;      /* debounced logical state: true = pressed */
    bool             last_raw_state;    /* last sampled logical state */
    bool             pressed_event;     /* one-shot press event */

    uint32_t         last_change_tick;  /* debounce timing */
    uint32_t         debounce_ms;       /* debounce period */
} button_t;

void button_init(button_t *me);
void button_update(button_t *me);

bool button_is_pressed(button_t *me);
bool button_was_pressed(button_t *me);

#endif /* BSP_BSP_BUTTON_BSP_BUTTON_H_ */
