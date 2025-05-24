#ifndef COMMS_H
#define COMMS_H

#include "stm32h7xx_hal.h"
#include <stdio.h>
#include "custom_typedefs.h"



// Definitions
#define CTS_PIN_NUM GPIO_PIN_0
#define CTS_PIN_GROUP  GPIOA

// Static variables & enums
typedef enum {
	DMA_COMPLETED = 0,
	DMA_INPROGRESS = 1
} DMA_status_flag;


static DMA_status_flag dma_flag;
static uint8_t data_buffer[61] = {0};

extern I2C_HandleTypeDef hi2c2;
extern SPI_HandleTypeDef hspi4;



// I2C comms.

void I2C_write(HAL_StatusTypeDef* status, DeviceAdress dev_adress, uint8_t reg_adress, uint8_t* data);
void I2C_read(HAL_StatusTypeDef* status, DeviceAdress dev_adress, uint8_t reg_adress, uint8_t* data);

// SPI utilities
void SPI_check_CTS(HAL_StatusTypeDef* status);


// SPI interrupt-based comms. (settings etc.)
void SPI_read(HAL_StatusTypeDef* status, uint8_t* data, uint8_t* size);
void SPI_write(HAL_StatusTypeDef* status, uint8_t* data, uint8_t* size);


// SPI DMA-based comms. (image data transfer)
void send_image(HAL_StatusTypeDef* status, uint8_t* data);
void receive_image(HAL_StatusTypeDef* status, uint8_t* data);
#endif
