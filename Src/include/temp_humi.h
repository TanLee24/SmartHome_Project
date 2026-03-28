#ifndef __TEMP_HUMI_H__
#define __TEMP_HUMI_H__

#include "DHT20.h"
#include "global.h"

// Task to Read Temperature and Humidity via DHT20 (Opptional: Display Data on LCD 16x2)
void temp_humi(void *pvParameters);

#endif