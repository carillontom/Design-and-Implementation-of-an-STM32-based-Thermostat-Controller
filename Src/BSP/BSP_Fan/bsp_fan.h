/*
 * bsp_fan.h
 *
 *  Created on: Mar 28, 2026
 *      Author: Tien Anh
 */

#ifndef BSP_BSP_FAN_BSP_FAN_H_
#define BSP_BSP_FAN_BSP_FAN_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    FAN_OFF = 0,
    FAN_ON
} fan_state_t;

typedef enum
{
    FAN_ACTIVE_HIGH = 0,
    FAN_ACTIVE_LOW
} fan_active_t;

typedef struct
{
    GPIO_TypeDef *port;
    uint8_t pin;                 /* pin number: 0..15 */
    fan_active_t active_level;   /* relay/driver active high or low */
    fan_state_t state;           /* software cached state */
} fan_t;

void fan_init(fan_t *me);
void fan_on(fan_t *me);
void fan_off(fan_t *me);
void fan_toggle(fan_t *me);
fan_state_t fan_get_state(fan_t *me);

#endif /* BSP_BSP_FAN_BSP_FAN_H_ */
