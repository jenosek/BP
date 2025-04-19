#include "comms.h"

HAL_StatusTypeDef comms_init(I2C_HandleTypeDef* I2C_handle) {
	p_I2C = I2C_handler;
}

HAL_StatusTypeDef write(DeviceAdress dev_adress, uint8_t reg_adress, uint8_t data) {
	HAL_StatusTypeDef status;

	status = HAL_I2C_Mem_Write(p_I2C, dev_adress, reg_adress, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
	HAL_Delay(70);

	return status;
}

HAL_StatusTypeDef camera_read(DeviceAdress dev_adress, uint8_t reg_adress, uint8_t* data) {
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Transmit(p_I2C, ++dev_adress, &adress, 1, 100);
	status |= HAL_I2C_Master_Receive(p_I2C, dev_adress, data, 1, 100);
	return status;
}


