#ifndef __TIMER_H__
#define __TIMER_H__

#include <errno.h>
#include <string.h>
#include "qapi_fs_types.h"
#include "qapi_status.h"
#include "qapi_diag.h"
//#include "quectel_utils.h"
//#include "quectel_uart_apis.h"
#include "../../main/log.h"

void timer1_handler(uint32_t data);
qapi_Status_t timer_init(void);
qapi_Status_t timer_start(void);

#endif