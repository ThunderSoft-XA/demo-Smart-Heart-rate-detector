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


#define QUERY_INTERVAL 5 //S

#define HEART_RATE_THRESHOLD	60		//心率阈值
#define X_AXIS_THRESHOLD		10		//x轴阈值
#define Y_AXIS_THRESHOLD		10		//y轴阈值
#define Z_AXIS_THRESHOLD		10		//z轴阈值
#define PRESSURE_THRESHOLD		50		//压力阈值



extern dataPoint_t currentDataPoint;
extern MODULE_PIN_ENUM  led_pin_num_motion ;
extern MODULE_PIN_ENUM  led_pin_num_Pulsesensor ;
extern MODULE_PIN_ENUM  led_pin_num_pressure ;
extern MODULE_PIN_ENUM  motor_pin_num ;

TX_TIMER *userTimer;

//led port
void led_init()
{
	   gizLog(LOG_INFO,"in led init...\n"); 
	   led_gpio_config();
}

void motor_init()
{
	   gizLog(LOG_INFO,"in motor_init ...\n"); 
	   motor_gpio_config();
}

//heartrate sensor init (Pulsesensor)
void heartrate_init()
{
	gizLog(LOG_INFO,"Pulsesensor initialization ...\n"); 
	Pulsesensor_init();
}

//ADXL345 init
void motion_init()
{
	gizLog(LOG_INFO,"ADXL345 initialization ...\n"); 
	//adxl345_init();
}

void pressure_init()
{
	gizLog(LOG_INFO,"fsr345 initialization ...\n"); 
	//fsr402_init();
}

void ICACHE_FLASH_ATTR userTimerCB(void)
{
	gizLog(LOG_INFO,"in userTimerCB.....\n"); 
    static uint8_t ctime = 0;
    static uint8_t ccount = 0;
    int8_t status = 0;
	
	uint32_t heartrate = 75;		//心跳
	
	//gizLog(LOG_INFO,"before QUERY_INTERVAL....\n"); 
	if (QUERY_INTERVAL < ctime)			
	{
			ctime = 0;				//come in per 7 times
			
			status = getHeartRateValue(&heartrate); 				//heartrate from Pulsesensor
			if( status )
			{
				gizLog(LOG_INFO,"get health info error\n"); 
				
			}
			if(currentDataPoint.valueHeartRateValue > HEART_RATE_THRESHOLD)		//心率超过阈值
			{
				//set_gpio_value(0x00, led_pin_num_Pulsesensor);
			}
			//数据上报
			//gizLog(LOG_INFO,"begin to upload data\n"); 
			gizwitsHandle((dataPoint_t *)&currentDataPoint);
			//gizLog(LOG_INFO,"currentDataPoint.valueHeartRateValue = %d, currentDataPoint.valueX_axis_Value = %d, currentDataPoint.valueY_axis_Value = %d, currentDataPoint.valueZ_axis_Value = %d, currentDataPoint.valuePressure_Value = %d,\n", 
			//urrentDataPoint.valueHeartRateValue, currentDataPoint.valueX_axis_Value,currentDataPoint.valueY_axis_Value,currentDataPoint.valueZ_axis_Value,currentDataPoint.valuePressure_Value);
			ccount++;				//record collect times
			gizLog(LOG_INFO,"ccount = %d\n", ccount); 
	}
    ctime++;  
}

void sensorInit(void)
{
    int32 ret = -1;
    
    gizLog(LOG_INFO,"Sensor initialization ...\n"); 

	led_init();								//led  init
	motor_init();						//motor init
	heartrate_init();					//plusensor init
	motion_init();						//adxl345 sensor init
	pressure_init();					//fsr402 init
    txm_module_object_allocate(&userTimer, sizeof(TX_TIMER));
    ret = tx_timer_create(userTimer, "userTimer", userTimerCB, NULL, 1,
                          100, TX_AUTO_ACTIVATE);			
    if(ret != TX_SUCCESS)
    {
        gizLog(LOG_WARNING,"Failed to create UserTimer.\n");
    }
}


void gagentMain( void )
{
    getFreeHeap();
    sensorInit();
    gizwitsInit();
}
