#ifndef ROUTINES_H
#define ROUTINES_H

#include "eps.h"
#include "radio_config_Si4468.h"
//#include "stm32h7xx_hal.h"

#include <stdio.h>
#include "radio.h"

//extern fault_flag error_index;

void setup(HAL_StatusTypeDef* status, fault_flag* error_index);
void get_radio_hw_info(HAL_StatusTypeDef* status, fault_flag* error_index);

#endif
