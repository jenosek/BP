#include "comms.h"

void comms_init(I2C_HandleTypeDef* I2C_handle) {
	p_I2C = I2C_handler;
	return;
}

void I2C_write(HAL_StatusTypeDef* status, DeviceAdress dev_adress, uint8_t reg_adress, uint8_t* data) {
	HAL_StatusTypeDef status;

	*status = HAL_I2C_Mem_Write(p_I2C, dev_adress, reg_adress, I2C_MEMADD_SIZE_8BIT, data, 1, 100);
	HAL_Delay(70);
	return;
}

void I2C_read(HAL_StatusTypeDef* status, DeviceAdress dev_adress, uint8_t reg_adress, uint8_t* data) {
	HAL_StatusTypeDef status;
	*status = HAL_I2C_Master_Transmit(p_I2C, ++dev_adress, &adress, 1, 100);
	*status |= HAL_I2C_Master_Receive(p_I2C, dev_adress, data, 1, 100);
	return;
}


