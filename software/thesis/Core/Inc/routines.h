#ifndef ROUTINES_H
#define ROUTINES_H

#include "eps.h"
#include "radio_config_Si4468.h"
//#include "stm32h7xx_hal.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include "radio.h"
//#include "ov7670.h"



void setup(HAL_StatusTypeDef* status, fault_flag* error_index);
void cmd_main_win();
void cmd_img_options();
void radio_ping(HAL_StatusTypeDef* status);
void send_img(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* img_mode);
void nirq_handler(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* ping);
void get_GS_state(HAL_StatusTypeDef* status);

//char** get_radio_hw_info(HAL_StatusTypeDef* status, fault_flag* error_index);

#endif
