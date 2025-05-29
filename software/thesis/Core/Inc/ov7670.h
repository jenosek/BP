#ifndef OV7670_H
#define OV7670_H

#include "custom_typedefs.h"
#include "comms.h"

/* LINKED EXTERNAL HANDLE
*****************************************/
extern DCMI_HandleTypeDef hdcmi;

/* ADRESSES OF REGISTERS
*****************************************/
// Communication modes
#define CAM_WRITE	0x42
#define CAM_READ	0x43

// Settings
#define CAM_COM2	0x09
#define CAM_COM3	0x0C
#define CAM_CLKRC	0x11
#define CAM_COM7	0x12
#define CAM_COM8 	0x13
#define CAM_COM10	0x15
#define CAM_MVFP	0x1E
#define CAM_TSLB 	0x3A
#define CAM_COM13	0x3D
#define CAM_COM14	0x3E
#define CAM_COM15 	0x40
#define CAM_MANU	0x67
#define CAM_MANV	0x68
#define CAM_DBLV	0x6B
#define CAM_RSVD	0xB0

// Testing patterns
#define CAM_XSC		0x70
#define CAM_YSC		0x71



/* PINOUT
*****************************************/
// Camera ports
#define CAM_PWR_PORT 	GPIOE
#define CAM_PWDN_PORT	GPIOD
#define CAM_RESET_PORT 	GPIOD

// Camera pins
#define CAM_PWR_PIN 	GPIO_PIN_15
#define CAM_PWDN_PIN	GPIO_PIN_10
#define CAM_RESET_PIN 	GPIO_PIN_9







/* FUNCTIONS
*****************************************/
// Communication with camera's registers
//HAL_StatusTypeDef camera_write(uint8_t adress, uint8_t data);
//HAL_StatusTypeDef camera_read(uint8_t adress, uint8_t* data);

// Camera control
void camera_init(HAL_StatusTypeDef* status, uint8_t* mode);
void camera_capture_photo(HAL_StatusTypeDef* status, uint8_t* destination_adress, uint32_t* size);
void camera_shut_down(HAL_StatusTypeDef* status);

#endif
