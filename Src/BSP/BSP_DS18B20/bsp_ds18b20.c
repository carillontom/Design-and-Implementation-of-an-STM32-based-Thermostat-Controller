/*
 * bsp_ds18b20.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */

#include "bsp_ds18b20.h"
#include "delay.h"
#include <stddef.h>

#define DS18B20_CMD_SKIP_ROM           0xCCU
#define DS18B20_CMD_CONVERT_T          0x44U
#define DS18B20_CMD_READ_SCRATCHPAD    0xBEU
#define DS18B20_CMD_WRITE_SCRATCHPAD   0x4EU

/* default alarm register values */
#define DS18B20_DEFAULT_TH             0x4BU
#define DS18B20_DEFAULT_TL             0x46U

static void ds18b20_set_pin_output(ds18b20_t *me);
static void ds18b20_set_pin_input(ds18b20_t *me);
static void ds18b20_write_pin(ds18b20_t *me, bool level);
static bool ds18b20_read_pin(ds18b20_t *me);

static void ds18b20_write_bit(ds18b20_t *me, uint8_t bit);
static uint8_t ds18b20_read_bit(ds18b20_t *me);
static void ds18b20_write_byte(ds18b20_t *me, uint8_t data);
static uint8_t ds18b20_read_byte(ds18b20_t *me);

static uint8_t ds18b20_crc8(const uint8_t *data, uint8_t len);
static bool ds18b20_read_scratchpad(ds18b20_t *me, uint8_t scratchpad[9]);
static int16_t ds18b20_raw_from_scratchpad(const uint8_t scratchpad[9]);
static int ds18b20_raw_to_int_celsius(int16_t raw_temp);

void ds18b20_init(ds18b20_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin >= 16U))
    {
        return;
    }

    me->resolution = DS18B20_RES_11_BIT;
    me->conversion_time_ms = ds18b20_get_conversion_time_ms(me->resolution);

    me->state = DS18B20_STATE_IDLE;
    me->conv_start_tick = 0U;

    me->last_temp_int = 0;
    me->data_valid = false;
    me->new_data_ready = false;

    ds18b20_set_pin_input(me);

    /* configure default resolution at startup */
    (void)ds18b20_set_resolution(me, me->resolution);
}

uint32_t ds18b20_get_conversion_time_ms(ds18b20_resolution_t resolution)
{
    switch (resolution)
    {
        case DS18B20_RES_9_BIT:
            return 94U;

        case DS18B20_RES_10_BIT:
            return 188U;

        case DS18B20_RES_11_BIT:
            return 375U;

        case DS18B20_RES_12_BIT:
        default:
            return 750U;
    }
}

bool ds18b20_set_resolution(ds18b20_t *me, ds18b20_resolution_t resolution)
{
    uint8_t config;

    if ((me == NULL) || (me->port == NULL) || (me->pin >= 16U))
    {
        return false;
    }

    switch (resolution)
    {
        case DS18B20_RES_9_BIT:
            config = 0x1FU;
            break;

        case DS18B20_RES_10_BIT:
            config = 0x3FU;
            break;

        case DS18B20_RES_11_BIT:
            config = 0x5FU;
            break;

        case DS18B20_RES_12_BIT:
        default:
            config = 0x7FU;
            break;
    }

    if (ds18b20_reset(me) == false)
    {
        return false;
    }

    ds18b20_write_byte(me, DS18B20_CMD_SKIP_ROM);
    ds18b20_write_byte(me, DS18B20_CMD_WRITE_SCRATCHPAD);
    ds18b20_write_byte(me, DS18B20_DEFAULT_TH);
    ds18b20_write_byte(me, DS18B20_DEFAULT_TL);
    ds18b20_write_byte(me, config);

    me->resolution = resolution;
    me->conversion_time_ms = ds18b20_get_conversion_time_ms(resolution);

    return true;
}

bool ds18b20_reset(ds18b20_t *me)
{
    bool presence;

    if ((me == NULL) || (me->port == NULL) || (me->pin >= 16U))
    {
        return false;
    }

    ds18b20_set_pin_output(me);
    ds18b20_write_pin(me, false);
    delay_us(480U);

    ds18b20_set_pin_input(me);
    delay_us(40U);

    /* presence pulse: sensor pulls line low */
    presence = (ds18b20_read_pin(me) == false);

    delay_us(420U);

    return presence;
}

bool ds18b20_start_conversion(ds18b20_t *me)
{
    if ((me == NULL) || (me->port == NULL) || (me->pin >= 16U))
    {
        return false;
    }

    if (ds18b20_reset(me) == false)
    {
        return false;
    }

    ds18b20_write_byte(me, DS18B20_CMD_SKIP_ROM);
    ds18b20_write_byte(me, DS18B20_CMD_CONVERT_T);

    return true;
}

bool ds18b20_read_temp_after_conversion(ds18b20_t *me, int *temp_c)
{
    uint8_t scratchpad[9];
    int16_t raw_temp;
    int temp_int;

    if ((me == NULL) || (temp_c == NULL))
    {
        return false;
    }

    if (ds18b20_read_scratchpad(me, scratchpad) == false)
    {
        me->data_valid = false;
        me->new_data_ready = false;
        return false;
    }

    raw_temp = ds18b20_raw_from_scratchpad(scratchpad);
    temp_int = ds18b20_raw_to_int_celsius(raw_temp);

    *temp_c = temp_int;
    me->last_temp_int = temp_int;
    me->data_valid = true;
    me->new_data_ready = true;

    return true;
}

bool ds18b20_read_temp_celsius(ds18b20_t *me, float *temp_c)
{
    uint8_t scratchpad[9];
    int16_t raw_temp;

    if ((me == NULL) || (temp_c == NULL))
    {
        return false;
    }

    if (ds18b20_start_conversion(me) == false)
    {
        me->data_valid = false;
        return false;
    }

    delay_ms(me->conversion_time_ms);

    if (ds18b20_read_scratchpad(me, scratchpad) == false)
    {
        me->data_valid = false;
        return false;
    }

    raw_temp = ds18b20_raw_from_scratchpad(scratchpad);
    *temp_c = (float)raw_temp / 16.0f;

    me->last_temp_int = ds18b20_raw_to_int_celsius(raw_temp);
    me->data_valid = true;
    me->new_data_ready = true;

    return true;
}

bool ds18b20_read_temp_int(ds18b20_t *me, int *temp_c)
{
    int temp_int;

    if ((me == NULL) || (temp_c == NULL))
    {
        return false;
    }

    if (ds18b20_start_conversion(me) == false)
    {
        me->data_valid = false;
        return false;
    }

    delay_ms(me->conversion_time_ms);

    if (ds18b20_read_temp_after_conversion(me, &temp_int) == false)
    {
        return false;
    }

    *temp_c = temp_int;
    return true;
}

bool ds18b20_request_sample(ds18b20_t *me, uint32_t now)
{
    if (me == NULL)
    {
        return false;
    }

    if (me->state != DS18B20_STATE_IDLE)
    {
        return false;
    }

    if (ds18b20_start_conversion(me) == false)
    {
        me->data_valid = false;
        me->new_data_ready = false;
        return false;
    }

    me->conv_start_tick = now;
    me->state = DS18B20_STATE_WAIT_CONVERSION;
    me->new_data_ready = false;

    return true;
}

void ds18b20_process(ds18b20_t *me, uint32_t now)
{
    int temp_int;

    if (me == NULL)
    {
        return;
    }

    switch (me->state)
    {
        case DS18B20_STATE_IDLE:
        {
            break;
        }

        case DS18B20_STATE_WAIT_CONVERSION:
        {
            if ((now - me->conv_start_tick) >= me->conversion_time_ms)
            {
                if (ds18b20_read_temp_after_conversion(me, &temp_int) == false)
                {
                    me->data_valid = false;
                    me->new_data_ready = false;
                }

                me->state = DS18B20_STATE_IDLE;
            }
            break;
        }

        default:
        {
            me->state = DS18B20_STATE_IDLE;
            break;
        }
    }
}

bool ds18b20_is_busy(ds18b20_t *me)
{
    if (me == NULL)
    {
        return false;
    }

    return (me->state != DS18B20_STATE_IDLE);
}

bool ds18b20_is_new_data_ready(ds18b20_t *me)
{
    if (me == NULL)
    {
        return false;
    }

    return me->new_data_ready;
}

bool ds18b20_get_last_temp_int(ds18b20_t *me, int *temp_c)
{
    if ((me == NULL) || (temp_c == NULL))
    {
        return false;
    }

    if (me->data_valid == false)
    {
        return false;
    }

    *temp_c = me->last_temp_int;
    me->new_data_ready = false;

    return true;
}

bool ds18b20_is_data_valid(ds18b20_t *me)
{
    if (me == NULL)
    {
        return false;
    }

    return me->data_valid;
}

static void ds18b20_write_bit(ds18b20_t *me, uint8_t bit)
{
    ds18b20_set_pin_output(me);
    ds18b20_write_pin(me, false);

    if (bit != 0U)
    {
        delay_us(6U);
        ds18b20_set_pin_input(me);
        delay_us(64U);
    }
    else
    {
        delay_us(60U);
        ds18b20_set_pin_input(me);
        delay_us(10U);
    }
}

static uint8_t ds18b20_read_bit(ds18b20_t *me)
{
    uint8_t bit_value;

    ds18b20_set_pin_output(me);
    ds18b20_write_pin(me, false);
    delay_us(3U);

    ds18b20_set_pin_input(me);
    delay_us(12U);

    bit_value = (ds18b20_read_pin(me) == true) ? 1U : 0U;

    delay_us(50U);

    return bit_value;
}

static void ds18b20_write_byte(ds18b20_t *me, uint8_t data)
{
    uint8_t i;

    for (i = 0U; i < 8U; i++)
    {
        ds18b20_write_bit(me, (uint8_t)((data >> i) & 0x01U));
    }
}

static uint8_t ds18b20_read_byte(ds18b20_t *me)
{
    uint8_t i;
    uint8_t value = 0U;

    for (i = 0U; i < 8U; i++)
    {
        value |= (uint8_t)(ds18b20_read_bit(me) << i);
    }

    return value;
}

static void ds18b20_write_pin(ds18b20_t *me, bool level)
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

static bool ds18b20_read_pin(ds18b20_t *me)
{
    return (((me->port->IDR & (1UL << me->pin)) != 0U) ? true : false);
}

static void ds18b20_set_pin_output(ds18b20_t *me)
{
    uint32_t shift;

    shift = (uint32_t)me->pin * 2U;

    /* MODER: 01 = output */
    me->port->MODER &= ~(3UL << shift);
    me->port->MODER |=  (1UL << shift);

    /* OTYPER: 1 = open-drain */
    me->port->OTYPER |= (1UL << me->pin);

    /* low speed is enough */
    me->port->OSPEEDR &= ~(3UL << shift);

    /* no pull while output */
    me->port->PUPDR &= ~(3UL << shift);
}

static void ds18b20_set_pin_input(ds18b20_t *me)
{
    uint32_t shift;

    shift = (uint32_t)me->pin * 2U;

    /* MODER: 00 = input */
    me->port->MODER &= ~(3UL << shift);

    /* PUPDR: 01 = pull-up */
    me->port->PUPDR &= ~(3UL << shift);
    me->port->PUPDR |=  (1UL << shift);
}

static uint8_t ds18b20_crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0U;
    uint8_t i;
    uint8_t j;
    uint8_t mix;

    for (i = 0U; i < len; i++)
    {
        uint8_t in_byte = data[i];

        for (j = 0U; j < 8U; j++)
        {
            mix = (uint8_t)((crc ^ in_byte) & 0x01U);
            crc >>= 1U;

            if (mix != 0U)
            {
                crc ^= 0x8CU;
            }

            in_byte >>= 1U;
        }
    }

    return crc;
}

static bool ds18b20_read_scratchpad(ds18b20_t *me, uint8_t scratchpad[9])
{
    uint8_t i;

    if ((me == NULL) || (scratchpad == NULL))
    {
        return false;
    }

    if (ds18b20_reset(me) == false)
    {
        return false;
    }

    ds18b20_write_byte(me, DS18B20_CMD_SKIP_ROM);
    ds18b20_write_byte(me, DS18B20_CMD_READ_SCRATCHPAD);

    for (i = 0U; i < 9U; i++)
    {
        scratchpad[i] = ds18b20_read_byte(me);
    }

    if (ds18b20_crc8(scratchpad, 8U) != scratchpad[8])
    {
        return false;
    }

    return true;
}

static int16_t ds18b20_raw_from_scratchpad(const uint8_t scratchpad[9])
{
    return (int16_t)(((uint16_t)scratchpad[1] << 8U) | scratchpad[0]);
}

static int ds18b20_raw_to_int_celsius(int16_t raw_temp)
{
    float temp_float;

    temp_float = (float)raw_temp / 16.0f;

    if (temp_float >= 0.0f)
    {
        return (int)(temp_float + 0.5f);
    }
    else
    {
        return (int)(temp_float - 0.5f);
    }
}
