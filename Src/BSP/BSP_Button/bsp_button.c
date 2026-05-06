/*
 * bsp_button.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */


#include "bsp_button.h"
#include "systick.h"
#include <stddef.h>

static bool button_read_raw(button_t *me);

void button_init(button_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin >= 16U))
    {
        return;
    }

    me->pressed_event    = false;
    me->last_change_tick = systick_get_tick();

    me->last_raw_state   = button_read_raw(me);
    me->stable_state     = me->last_raw_state;
}

void button_update(button_t *me)
{
    bool raw_state;
    uint32_t now;

    if ((me == NULL) || (me->port == NULL) || (me->pin >= 16U))
    {
        return;
    }

    raw_state = button_read_raw(me);
    now = systick_get_tick();

    /* raw input changed -> restart debounce timer */
    if (raw_state != me->last_raw_state)
    {
        me->last_raw_state = raw_state;
        me->last_change_tick = now;
    }

    /* if raw input stays stable long enough, accept it */
    if ((now - me->last_change_tick) >= me->debounce_ms)
    {
        if (me->stable_state != me->last_raw_state)
        {
            me->stable_state = me->last_raw_state;

            /* generate one-shot event on press only */
            if (me->stable_state == true)
            {
                me->pressed_event = true;
            }
        }
    }
}

bool button_is_pressed(button_t *me)
{
    if (me == NULL)
    {
        return false;
    }

    return me->stable_state;
}

bool button_was_pressed(button_t *me)
{
    bool event;

    if (me == NULL)
    {
        return false;
    }

    event = me->pressed_event;
    me->pressed_event = false;

    return event;
}

static bool button_read_raw(button_t *me)
{
    bool pin_level;

    pin_level = (((me->port->IDR & (1UL << me->pin)) != 0U) ? true : false);

    if (me->active_level == BUTTON_ACTIVE_LOW)
    {
        return (!pin_level);
    }
    else
    {
        return pin_level;
    }
}
