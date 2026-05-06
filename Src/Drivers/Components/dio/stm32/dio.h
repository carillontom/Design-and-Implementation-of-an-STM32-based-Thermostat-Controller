/*
 * dio.h
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */

#ifndef DRIVER_COMPONENTS_DIO_DO_H_
#define DRIVER_COMPONENTS_DIO_DO_H_

#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    bool bStatus;
} do_t;

void do_set(do_t *me);
void do_reset(do_t *me);
void do_toggle(do_t *me);
bool do_get_status(do_t *me);

#endif /* DRIVER_COMPONENTS_DIO_DO_H_ */
