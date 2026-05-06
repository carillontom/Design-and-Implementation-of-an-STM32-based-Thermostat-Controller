/*
 * bsp_ds18b20.h
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */

#ifndef BSP_BSP_DS18B20_BSP_DS18B20_H_
#define BSP_BSP_DS18B20_BSP_DS18B20_H_

#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    DS18B20_RES_9_BIT = 0,
    DS18B20_RES_10_BIT,
    DS18B20_RES_11_BIT,
    DS18B20_RES_12_BIT
} ds18b20_resolution_t;

typedef enum
{
    DS18B20_STATE_IDLE = 0,
    DS18B20_STATE_WAIT_CONVERSION
} ds18b20_state_t;

typedef struct
{
    GPIO_TypeDef *port;
    uint8_t pin;                       /* pin number: 0..15 */

    ds18b20_resolution_t resolution;
    uint32_t conversion_time_ms;

    ds18b20_state_t state;
    uint32_t conv_start_tick;

    int last_temp_int;
    bool data_valid;
    bool new_data_ready;
} ds18b20_t;

/* init / config */
void ds18b20_init(ds18b20_t *me);
bool ds18b20_set_resolution(ds18b20_t *me, ds18b20_resolution_t resolution);
uint32_t ds18b20_get_conversion_time_ms(ds18b20_resolution_t resolution);

/* low-level protocol helpers */
bool ds18b20_reset(ds18b20_t *me);
bool ds18b20_start_conversion(ds18b20_t *me);
bool ds18b20_read_temp_after_conversion(ds18b20_t *me, int *temp_c);

/* blocking compatibility APIs */
bool ds18b20_read_temp_celsius(ds18b20_t *me, float *temp_c);
bool ds18b20_read_temp_int(ds18b20_t *me, int *temp_c);

/* non-blocking driver task style */
bool ds18b20_request_sample(ds18b20_t *me, uint32_t now);
void ds18b20_process(ds18b20_t *me, uint32_t now);

bool ds18b20_is_busy(ds18b20_t *me);
bool ds18b20_is_new_data_ready(ds18b20_t *me);
bool ds18b20_get_last_temp_int(ds18b20_t *me, int *temp_c);
bool ds18b20_is_data_valid(ds18b20_t *me);

#endif /* BSP_BSP_DS18B20_BSP_DS18B20_H_ */
