/*
 * bsp_fan.c
 *
 *  Created on: Mar 28, 2026
 *      Author: Tien Anh
 */

#include "bsp_fan.h"
#include <stddef.h>

static void fan_write_pin(fan_t *me, bool level);

void fan_init(fan_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin >= 16U))
    {
        return;
    }

    fan_off(me);
}

void fan_on(fan_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin >= 16U))
    {
        return;
    }

    if (me->active_level == FAN_ACTIVE_HIGH)
    {
        fan_write_pin(me, true);
    }
    else
    {
        fan_write_pin(me, false);
    }

    me->state = FAN_ON;
}

void fan_off(fan_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin >= 16U))
    {
        return;
    }

    if (me->active_level == FAN_ACTIVE_HIGH)
    {
        fan_write_pin(me, false);
    }
    else
    {
        fan_write_pin(me, true);
    }

    me->state = FAN_OFF;
}

void fan_toggle(fan_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin >= 16U))
    {
        return;
    }

    if (me->state == FAN_ON)
    {
        fan_off(me);
    }
    else
    {
        fan_on(me);
    }
}

fan_state_t fan_get_state(fan_t *me)
{
    if (me == NULL)
    {
        return FAN_OFF;
    }

    return me->state;
}

static void fan_write_pin(fan_t *me, bool level)
{
    if (level == true)
    {
        me->port->BSRR = (1UL << me->pin);
    }
    else
    {
        me->port->BSRR = (1UL << (me->pin + 16U));
    }
}
