#ifndef _PLUSENSOR_H_
#define _PLUSENSOR_H_

#include <stdint.h>
#include "../adc/adc.h"

void Pulsesensor_init();
uint8_t getHeartRateValue(uint32_t* heartrate);


#endif   _PLUSENSOR_H_
