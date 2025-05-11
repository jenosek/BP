#include "comms.h"

void comms_init(I2C_HandleTypeDef* I2C_handle, SPI_HandleTypeDef* SPI_handle, DMA_HandleTypeDef* SPI_DMA_Tx, DMA_HandleTypeDef* SPI_DMA_Rx) {
	p_I2C = I2C_handle;
	p_SPI = SPI_handle;
	p_SPI_DMA_Tx = SPI_DMA_Tx;
	p_SPI_DMA_Rx = SPI_DMA_Rx;
	return;
}

// DMA handler
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {

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

void SPI_read(HAL_StatusTypeDef* status, uint8_t* data) {
	// Receive data array of given size defined by size of provided array
	// Maximum data length provided by SI chip is 16 bytes (1 message = 1 byte)
	// Repetitive reading (from 1 byte) is possible

	// First transmit read request (only first byte - ID)
	*status |= HAL_SPI_Transmit(p_SPI, data, 1, 50);

	// Rewrite response into array
	*status |= HAL_SPI_Receive(p_SPI, data, sizeof(data)/8, 50);
	return;
}


void SPI_write(HAL_StatusTypeDef* status, uint8_t* data, uint8_t* response) {
	// Transmit data array of given size (message elements)
	*status |= HAL_SPI_Transmit(p_SPI, data, sizeof(data)/8, 50);
	return;
}


void send_image(HAL_StatusTypeDef* status, uint8_t* data) {

}
void receive_image(HAL_StatusTypeDef* status, uint8_t* data);

