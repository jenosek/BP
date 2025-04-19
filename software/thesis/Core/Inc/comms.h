#ifndef COMMS_H
#define COMMS_H


typedef enum
{
  CAMERA = 0x42,
  USB_C_CONTROLLER = 0b10000100,
} DeviceAdress;

static I2C_HandleTypeDef* p_I2C;

HAL_StatusTypeDef comms_init(I2C_HandleTypeDef* I2C_handle);
HAL_StatusTypeDef write(DeviceAdress dev_adress, uint8_t reg_adress, uint8_t data);
HAL_StatusTypeDef read(DeviceAdress dev_adress, uint8_t reg_adress, uint8_t* data);

#endif
