/*
 * bsp_lcd.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */

#include "bsp_lcd.h"
#include "delay.h"
#include <stddef.h>

static void lcd_write_pin(GPIO_TypeDef *port, uint8_t pin, uint8_t value);
static void lcd_pulse_enable(lcd_t *me);
static void lcd_write_4bits(lcd_t *me, uint8_t nibble);

void lcd_init(lcd_t *me)
{
    if (me == NULL)
    {
        return;
    }

    delay_ms(40U);

    /* Initialization sequence for 4-bit mode */
    lcd_write_pin(me->rs_port, me->rs_pin, 0U);
    lcd_write_4bits(me, 0x03U);
    delay_ms(5U);

    lcd_write_4bits(me, 0x03U);
    delay_us(150U);

    lcd_write_4bits(me, 0x03U);
    delay_us(150U);

    lcd_write_4bits(me, 0x02U);   /* switch to 4-bit mode */
    delay_us(150U);

    lcd_send_cmd(me, 0x28U);      /* 4-bit, 2 lines, 5x8 font */
    lcd_send_cmd(me, 0x0CU);      /* display ON, cursor OFF */
    lcd_send_cmd(me, 0x06U);      /* entry mode: increment cursor */
    lcd_send_cmd(me, 0x01U);      /* clear display */
    delay_ms(2U);
}

void lcd_clear(lcd_t *me)
{
    if (me == NULL)
    {
        return;
    }

    lcd_send_cmd(me, 0x01U);
    delay_ms(2U);
}

void lcd_send_cmd(lcd_t *me, uint8_t cmd)
{
    if (me == NULL)
    {
        return;
    }

    lcd_write_pin(me->rs_port, me->rs_pin, 0U);

    lcd_write_4bits(me, (uint8_t)((cmd >> 4) & 0x0FU));
    lcd_write_4bits(me, (uint8_t)(cmd & 0x0FU));

    delay_us(50U);
}

void lcd_send_data(lcd_t *me, uint8_t data)
{
    if (me == NULL)
    {
        return;
    }

    lcd_write_pin(me->rs_port, me->rs_pin, 1U);

    lcd_write_4bits(me, (uint8_t)((data >> 4) & 0x0FU));
    lcd_write_4bits(me, (uint8_t)(data & 0x0FU));

    delay_us(50U);
}

void lcd_set_cursor(lcd_t *me, uint8_t row, uint8_t col)
{
    uint8_t address;

    if (me == NULL)
    {
        return;
    }

    switch (row)
    {
        case 0:
            address = 0x00U + col;
            break;

        case 1:
            address = 0x40U + col;
            break;

        default:
            address = 0x00U + col;
            break;
    }

    lcd_send_cmd(me, (uint8_t)(0x80U | address));
}

void lcd_print_str(lcd_t *me, const char *str)
{
    if ((me == NULL) || (str == NULL))
    {
        return;
    }

    while (*str != '\0')
    {
        lcd_send_data(me, (uint8_t)(*str));
        str++;
    }
}

void lcd_print_int(lcd_t *me, int value)
{
    char buffer[12];
    int i = 0;
    int j;
    int temp;
    int is_negative = 0;

    if (me == NULL)
    {
        return;
    }

    if (value == 0)
    {
        lcd_send_data(me, '0');
        return;
    }

    if (value < 0)
    {
        is_negative = 1;
        value = -value;
    }

    while (value > 0)
    {
        temp = value % 10;
        buffer[i++] = (char)('0' + temp);
        value /= 10;
    }

    if (is_negative)
    {
        buffer[i++] = '-';
    }

    for (j = i - 1; j >= 0; j--)
    {
        lcd_send_data(me, (uint8_t)buffer[j]);
    }
}

static void lcd_write_pin(GPIO_TypeDef *port, uint8_t pin, uint8_t value)
{
    if ((port == NULL) || (pin >= 16U))
    {
        return;
    }

    if (value != 0U)
    {
        port->BSRR = (1UL << pin);
    }
    else
    {
        port->BSRR = (1UL << (pin + 16U));
    }
}

static void lcd_pulse_enable(lcd_t *me)
{
    lcd_write_pin(me->en_port, me->en_pin, 1U);
    delay_us(2U);

    lcd_write_pin(me->en_port, me->en_pin, 0U);
    delay_us(50U);
}

static void lcd_write_4bits(lcd_t *me, uint8_t nibble)
{
    lcd_write_pin(me->d4_port, me->d4_pin, (uint8_t)((nibble >> 0) & 0x01U));
    lcd_write_pin(me->d5_port, me->d5_pin, (uint8_t)((nibble >> 1) & 0x01U));
    lcd_write_pin(me->d6_port, me->d6_pin, (uint8_t)((nibble >> 2) & 0x01U));
    lcd_write_pin(me->d7_port, me->d7_pin, (uint8_t)((nibble >> 3) & 0x01U));

    lcd_pulse_enable(me);
}
