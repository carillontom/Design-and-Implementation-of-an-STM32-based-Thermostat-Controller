/*
 * app_main.c
 *
 *  Created on: Mar 27, 2026
 *      Author: Tien Anh
 */


#include "app_main.h"
#include "bsp_lcd.h"
#include "bsp_ds18b20.h"
#include "bsp_button.h"
#include "bsp_fan.h"
#include "delay.h"
#include "systick.h"
#include <stdbool.h>

//state declaration
typedef enum
{
    APP_STATE_NORMAL = 0,
    APP_STATE_SETTING
} app_state_t;

typedef enum
{
    TEMP_STATE_IDLE = 0,
    TEMP_STATE_WAIT_CONVERSION
} temp_state_t;

//object declaration
static lcd_t lcd = {
	.rs_port = GPIOA,
	.rs_pin = 2,

	.en_port = GPIOA,
	.en_pin = 3,

	.d4_port = GPIOA,
	.d4_pin = 4,

	.d5_port = GPIOA,
	.d5_pin = 5,

	.d6_port = GPIOA,
	.d6_pin = 6,

	.d7_port = GPIOA,
	.d7_pin = 7
};

static ds18b20_t sensor = {
    .port = GPIOB,
    .pin  = 10
};

static button_t button_up = {
	.port = GPIOB,
	.pin = 0,
	.active_level = BUTTON_ACTIVE_LOW,

	.stable_state = false,
	.last_raw_state = false,
	.pressed_event = false,

	.last_change_tick = 0U,
	.debounce_ms = 20U
};

static button_t button_down = {
	.port = GPIOB,
	.pin = 1,
	.active_level = BUTTON_ACTIVE_LOW,

	.stable_state = false,
	.last_raw_state = false,
	.pressed_event = false,

	.last_change_tick = 0U,
	.debounce_ms = 20U
};

static button_t button_set = {
	.port = GPIOB,
	.pin = 2,
	.active_level = BUTTON_ACTIVE_LOW,

	.stable_state = false,
	.last_raw_state = false,
	.pressed_event = false,

	.last_change_tick = 0U,
	.debounce_ms = 20U
};

static fan_t fan = {
	.port = GPIOB,
	.pin = 5,
	.active_level = FAN_ACTIVE_HIGH,
	.state = FAN_OFF
};

//variable declaration
static app_state_t app_state;
static temp_state_t temp_state;

static int temp_current;
static int temp_edit;
static int temp_set;
static bool sensor_valid;

static uint32_t last_button_tick;
static uint32_t last_temp_tick;
static uint32_t last_lcd_tick;
static uint32_t temp_conv_start_tick;

//function declaration
static void app_handle_buttons(void);
static void app_handle_temperature(void);
static void app_handle_fsm(void);
static void app_handle_control(void);
static void app_update_lcd(void);

void app_main_init(void) {
	lcd_init(&lcd);
	ds18b20_init(&sensor);
	ds18b20_set_resolution(&sensor, DS18B20_RES_11_BIT);

	lcd_clear(&lcd);
	lcd_set_cursor(&lcd, 0, 0);

	button_init(&button_up);
	button_init(&button_down);
	button_init(&button_set);

	fan_init(&fan);

	app_state = APP_STATE_NORMAL;
	temp_state = TEMP_STATE_IDLE;
	temp_conv_start_tick = 0U;
	temp_current = 0;
	temp_edit = 25;
	temp_set = 25;
	sensor_valid = false;
};

void app_main_run(void) {
	app_handle_buttons();
	app_handle_temperature();
	app_handle_fsm();
	app_handle_control();
	app_update_lcd();
}

void app_handle_buttons() {
	uint32_t now = systick_get_tick();

	if (now - last_button_tick >= 10U) {
		last_button_tick = now;

		button_update(&button_up);
		button_update(&button_down);
		button_update(&button_set);
	};
}

static void app_handle_temperature(void)
{
    uint32_t now = systick_get_tick();
    int new_temp;

    ds18b20_process(&sensor, now);

    if ((now - last_temp_tick) >= 500U)
    {
        last_temp_tick = now;
        (void)ds18b20_request_sample(&sensor, now);
    }

    if (ds18b20_is_new_data_ready(&sensor))
    {
        if (ds18b20_get_last_temp_int(&sensor, &new_temp))
        {
            temp_current = new_temp;
            sensor_valid = true;
        }
        else
        {
            sensor_valid = false;
        }
    }
};

void app_handle_fsm() {
	switch (app_state) {
	case APP_STATE_NORMAL: {
		if (button_was_pressed(&button_set)) {
			temp_edit = temp_set;
			app_state = APP_STATE_SETTING;
		}
		break;
	}

	case APP_STATE_SETTING: {
		if (button_was_pressed(&button_up)) {
			if (temp_edit < 35) {
				temp_edit++;
			};
		};

		if (button_was_pressed(&button_down)) {
			if (temp_edit > 15) {
				temp_edit--;
			};
		};

		if (button_was_pressed(&button_set)) {
			temp_set = temp_edit;
			app_state = APP_STATE_NORMAL;
		};

		break;
	}

	default: {
		app_state = APP_STATE_NORMAL;
		break;
	}
	}
}

void app_handle_control() {
	if (temp_current > temp_set) {
		fan_on(&fan);
	} else {
		fan_off(&fan);
	};
}

void app_update_lcd() {
	uint32_t now = systick_get_tick();

	if (now - last_lcd_tick < 200U) {
		return;
	};

	last_lcd_tick = now;

	if (!sensor_valid) {
		return;
	};

	switch (app_state) {
	case APP_STATE_NORMAL: {
		lcd_set_cursor(&lcd, 0, 0);
		lcd_print_str(&lcd, "Temperature:");
		lcd_print_int(&lcd, temp_current);
		lcd_print_str(&lcd, " C");

		lcd_set_cursor(&lcd, 1, 0);
		lcd_print_str(&lcd, "Fan state:");
		if (fan_get_state(&fan)) {
			lcd_print_str(&lcd, "ON    ");
		} else {
			lcd_print_str(&lcd, "OFF   ");
		};
		break;
	}

	case APP_STATE_SETTING: {
		lcd_set_cursor(&lcd, 0, 0);
		lcd_print_str(&lcd, "SET TEMP TO:");
		lcd_print_int(&lcd, temp_edit);
		lcd_print_str(&lcd, " C");

		lcd_set_cursor(&lcd, 1, 0);
		lcd_print_str(&lcd, "Temperature:");
		lcd_print_int(&lcd, temp_current);
		lcd_print_str(&lcd, " C");
		break;
	}

	default: {
		lcd_set_cursor(&lcd, 0, 0);
		lcd_print_str(&lcd, "ERROR");
		break;
	}
	}
}
