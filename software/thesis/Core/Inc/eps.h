#ifndef EPS_H
#define EPS_H

#include "comms.h"

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_def.h"
#include <stdio.h>

/* FUSB302BMPX */
#define DEV_ID 0x01
#define CONTROL0 0x6
#define CONTROL2 0x8
#define RESET 0xC
#define SWITCHES0 0x2
#define MASK 0xA
#define MASKA 0xE
#define MASKB 0xF





char* GetDevID(HAL_StatusTypeDef* status);
void PowerReset(HAL_StatusTypeDef* status);
void PowerInit(HAL_StatusTypeDef* status);
void PowerIncreaseCurrent(HAL_StatusTypeDef* status);





#endif
