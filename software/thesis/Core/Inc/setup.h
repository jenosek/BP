#ifndef SETUP_H
#define SETUP_H

#include "eps.h"
#include "radio_config_Si4468.h"
//#include "stm32h7xx_hal.h"

#include <stdio.h>

//extern fault_flag error_index;

void setup(HAL_StatusTypeDef* status, fault_flag* error_index);


#endif
