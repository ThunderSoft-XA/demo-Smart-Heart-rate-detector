/******************************************************************************
*@file    example_adc.h
*@brief   read value of ADC0 and ADC1
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#ifndef __ADC_H__
#define __ADC_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include "qapi_fs_types.h"
#include "qapi_status.h"
#include "qapi_adc.h"

#include "qapi_uart.h"
#include "qapi_timer.h"
#include "qapi_diag.h"
//#include "quectel_utils.h"
//#include "quectel_uart_apis.h"
#include "../../main/log.h"

qapi_Status_t adc_open_handle(void);
qapi_Status_t adc_get_properties(const char *Channel_Name_Ptr,qapi_Adc_Input_Properties_Type_t *Properties_Ptr);
extern int qapi_custom_cb_ADC_dispatcher(UINT cb_id, void *app_cb, 
									 UINT cb_param1, UINT cb_param2, 
									 UINT cb_param3, UINT cb_param4,
									 UINT cb_param5, UINT cb_param6,
									 UINT cb_param7, UINT cb_param8);



#endif /*_ADC_H__*/

