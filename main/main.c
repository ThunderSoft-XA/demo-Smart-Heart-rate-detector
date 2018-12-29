/**
************************************************************
* @file         main.c
* @brief        main
* @author       Gizwits
* @date         2018-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#include "gagent_soc.h"
#include "../Gizwits/gizwits_product.h"
#include "log.h"
#include "../driver/gpio/gpio.h"
#include "../driver/plusensor/plusensor.h"

#define X_AXIS_THRESHOLD 10		//x轴阈值
#define Y_AXIS_THRESHOLD 10		//y轴阈值
#define Z_AXIS_THRESHOLD 10		//z轴阈值
#define PRESSURE_THRESHOLD 50   //压力阈值

extern dataPoint_t currentDataPoint;
/*
extern MODULE_PIN_ENUM led_pin_num_motion;
extern MODULE_PIN_ENUM led_pin_num_Pulsesensor;
extern MODULE_PIN_ENUM led_pin_num_pressure;
extern MODULE_PIN_ENUM motor_pin_num;
*/
extern MODULE_PIN_ENUM led_blue;
extern MODULE_PIN_ENUM led_red;
extern MODULE_PIN_ENUM led_green;

TX_TIMER *userTimer;

//led port
void led_init()
{
	gizLog(LOG_INFO, "in led init...\n");
	led_gpio_config();
	led_on_off(false, led_blue);
	led_on_off(false, led_red);
	led_on_off(true, led_green);
}

void motor_init()
{
	gizLog(LOG_INFO, "in motor_init ...\n");
	//motor_gpio_config();
}

//heartrate sensor init (Pulsesensor)
void heartrate_init()
{
	gizLog(LOG_INFO, "Pulsesensor initialization ...\n");
	//Pulsesensor_init();
}

//ADXL345 init
void motion_init()
{
	gizLog(LOG_INFO, "ADXL345 initialization ...\n");
	//adxl345_init();
}

void pressure_init()
{
	gizLog(LOG_INFO, "fsr345 initialization ...\n");
	//fsr402_init();
}

static uint32_t heartrate; 


void sensorInit(void)
{
	int32 ret = -1;
	gizLog(LOG_INFO, "Sensor initialization ...\n");
	led_init();
	Pulsesensor_init();
}



void gagentMain(void)
{
	getFreeHeap();
	sensorInit();
	gizwitsInit();
	timer_init();
	timer_start();
}
