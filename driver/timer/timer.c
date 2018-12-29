#include "timer.h"
#include "gpio.h"
#include "qapi_timer.h"

#define HEART_RATE_THRESHOLD_HIGH 100 //心率阈值最大值
#define HEART_RATE_THRESHOLD_LOW 60 //心率阈值最小值

qapi_TIMER_handle_t timer_handle;
qapi_TIMER_define_attr_t timer_def_attr;
qapi_TIMER_set_attr_t timer_set_attr;

extern MODULE_PIN_ENUM led_blue;
extern MODULE_PIN_ENUM led_red;
extern MODULE_PIN_ENUM led_green;

static int heartrate = 0;

void timer1_handler(uint32_t data)
{
	static bool led_red_status = true;
	getHeartRateValue(&heartrate);
	if(!heartrate)	//no useful value		
	{
		return;
	}
	else if(((heartrate > HEART_RATE_THRESHOLD_HIGH) || (heartrate < HEART_RATE_THRESHOLD_LOW)))		// value is not vaild
	{
		led_on_off(true, led_red);				//red on
		//led_on_off(led_red_status, led_red);		
		//led_red_status = !led_red_status;
	}
 	else																								// value is vaild
	{
		led_on_off(false,led_red);				//red off
	} 
}

qapi_Status_t timer_init(void) 
{
    qapi_Status_t status = QAPI_OK;

    memset(&timer_def_attr, 0, sizeof(timer_def_attr));
	timer_def_attr.cb_type	= QAPI_TIMER_FUNC1_CB_TYPE;
	timer_def_attr.deferrable = false;
	timer_def_attr.sigs_func_ptr = timer1_handler;
	timer_def_attr.sigs_mask_data = 0x11;
	status = qapi_Timer_Def(&timer_handle, &timer_def_attr);

    return status;
}

qapi_Status_t timer_start(void)
{
    qapi_Status_t status = QAPI_OK;

    memset(&timer_set_attr, 0, sizeof(timer_set_attr));
	timer_set_attr.reload = 100;
	timer_set_attr.time = 10;
	timer_set_attr.unit = QAPI_TIMER_UNIT_MSEC;
	status = qapi_Timer_Set(timer_handle, &timer_set_attr);

    return status;
}

