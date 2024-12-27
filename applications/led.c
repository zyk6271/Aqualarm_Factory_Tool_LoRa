#include <agile_led.h>
#include <stdlib.h>
#include "led.h"
#include "pin_config.h"
#include <agile_led.h>

static agile_led_t *led_obj_rf_green = RT_NULL;
static agile_led_t *led_obj_rf_red = RT_NULL;
static agile_led_t *led_obj_beep = RT_NULL;

#define DBG_TAG "LED"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

void led_init(void)
{
    led_obj_rf_green = agile_led_create(LED1_PIN, PIN_LOW, "200,200", -1);
    led_obj_rf_red = agile_led_create(LED2_PIN, PIN_LOW, "200,200", -1);
    led_obj_beep = agile_led_create(BUZZER_PIN, PIN_HIGH, "200,200", -1);

    agile_led_set_light_mode(led_obj_beep, "500,1", 1);
    agile_led_start(led_obj_beep);
}

void led_beep_start(uint8_t count)
{
    agile_led_stop(led_obj_beep);
    agile_led_set_light_mode(led_obj_beep, "200,200", count);
    agile_led_start(led_obj_beep);
}

void rf_led_resume(agile_led_t *led)
{
    agile_led_set_compelete_callback(led_obj_rf_green,RT_NULL);
    agile_led_set_light_mode(led_obj_rf_green, "200,0", -1);
    agile_led_start(led_obj_rf_green);
}

void rf_led(uint8_t type)
{
    switch(type)
    {
    case 0://RF初始化失败
        agile_led_stop(led_obj_rf_green);
        agile_led_set_light_mode(led_obj_rf_red, "200,0", -1);
        agile_led_start(led_obj_rf_red);
        break;
    case 1://RF初始化成功
        agile_led_stop(led_obj_rf_red);
        agile_led_set_light_mode(led_obj_rf_green, "200,0", -1);
        agile_led_start(led_obj_rf_green);
        break;
    case 2://RF发送
        agile_led_stop(led_obj_rf_red);
        agile_led_set_light_mode(led_obj_rf_green, "10,100",1);
        agile_led_set_compelete_callback(led_obj_rf_green,rf_led_resume);
        agile_led_start(led_obj_rf_green);
        break;
    case 3://RF接收
        agile_led_stop(led_obj_rf_red);
        agile_led_set_light_mode(led_obj_rf_green, "10,100",1);
        agile_led_set_compelete_callback(led_obj_rf_green,rf_led_resume);
        agile_led_start(led_obj_rf_green);
        break;
    }
}
