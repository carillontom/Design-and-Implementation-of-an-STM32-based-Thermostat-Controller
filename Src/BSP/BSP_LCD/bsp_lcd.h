/*
 * bsp_lcd.h
 *
 *  Created on: Mar 26, 2026
 *      Author: Tien Anh
 */

#ifndef BSP_BSP_LCD_BSP_LCD_H_
#define BSP_BSP_LCD_BSP_LCD_H_

#include "stm32f4xx.h"
#include <stdint.h>

typedef struct
{
    GPIO_TypeDef *rs_port;
    uint8_t       rs_pin;

    GPIO_TypeDef *en_port;
    uint8_t       en_pin;

    GPIO_TypeDef *d4_port;
    uint8_t       d4_pin;

    GPIO_TypeDef *d5_port;
    uint8_t       d5_pin;

    GPIO_TypeDef *d6_port;
    uint8_t       d6_pin;

    GPIO_TypeDef *d7_port;
    uint8_t       d7_pin;
} lcd_t;

void lcd_init(lcd_t *me);
void lcd_clear(lcd_t *me);
void lcd_send_cmd(lcd_t *me, uint8_t cmd);
void lcd_send_data(lcd_t *me, uint8_t data);
void lcd_set_cursor(lcd_t *me, uint8_t row, uint8_t col);
void lcd_print_str(lcd_t *me, const char *str);
void lcd_print_int(lcd_t *me, int value);

#endif /* BSP_BSP_LCD_BSP_LCD_H_ */
