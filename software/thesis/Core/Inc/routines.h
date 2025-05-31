#ifndef ROUTINES_H
#define ROUTINES_H

#include "eps.h"
#include "radio_config_Si4468.h"
//#include "stm32h7xx_hal.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include "radio.h"
//#include "ov7670.h"

extern uint32_t img_size;
extern uint8_t img_flag;

void setup(HAL_StatusTypeDef* status, fault_flag* error_index);
void cmd_main_win();
void cmd_img_options();
void radio_ping(HAL_StatusTypeDef* status);
void get_img_res(HAL_StatusTypeDef* status, uint8_t* img_mode);
void request_img(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* img_mode);
void capture_img(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* img_buffer, uint8_t* img_mode);
void transmit_img(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* img_buffer, uint32_t* index);
void receive_img(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* img);
void nirq_handler(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* ping, uint8_t* ack, uint8_t* img_buffer, uint32_t* index);
void get_GS_state(HAL_StatusTypeDef* status);

//char** get_radio_hw_info(HAL_StatusTypeDef* status, fault_flag* error_index);

#endif
