/*
 * dio.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */
#include "dio.h"

#include <stddef.h>

void do_set(do_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin == 0U))
    {
        return;
    }

    me->port->BSRR = (1UL << me->pin);
    me->bStatus = true;
}

void do_reset(do_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin == 0U))
    {
        return;
    }

    me->port->BSRR = (1UL << (me->pin << 16U));
    me->bStatus = false;
}

void do_toggle(do_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin == 0U))
    {
        return;
    }

    if ((me->port->ODR & (1UL << me->pin)) != 0U)
    {
        me->port->BSRR = (1UL << (me->pin + 16U));
        me->bStatus = false;
    }
    else
    {
        me->port->BSRR = (1UL << me->pin);
        me->bStatus = true;
    }
}

bool do_get_status(do_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin == 0U))
    {
        return false;
    }

    if ((me->port->ODR & (1UL << me->pin)) != 0U) {
    	return true;
    } else {
    	return false;
    }
}

