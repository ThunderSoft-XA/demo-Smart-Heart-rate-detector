/******************************************************************************
*@file    motor.h
*@brief   example of led operation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#ifndef __GPIO_H__
#define __GPIO_H__

#include "quectel_gpio.h"

 void led_gpio_config();
 void motor_gpio_config();
 void  set_gpio_value(uint8_t ledvalue, MODULE_PIN_ENUM m_pin);
void gpio_config(MODULE_PIN_ENUM m_pin,qapi_GPIO_Direction_t gpio_dir,qapi_GPIO_Pull_t gpio_pull,qapi_GPIO_Drive_t gpio_drive);

#endif
