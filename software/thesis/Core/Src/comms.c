#include "comms.h"


// I2C
void I2C_write(HAL_StatusTypeDef* status, DeviceAdress dev_adress, uint8_t reg_adress, uint8_t* data) {
	*status = HAL_I2C_Mem_Write(&hi2c2, dev_adress, reg_adress, I2C_MEMADD_SIZE_8BIT, data, 1, 100);
}

void I2C_read(HAL_StatusTypeDef* status, DeviceAdress dev_adress, uint8_t reg_adress, uint8_t* data) {
	dev_adress += 1;
	*status = HAL_I2C_Master_Transmit(&hi2c2, dev_adress, &reg_adress, 1, 500);
	*status |= HAL_I2C_Master_Receive(&hi2c2, dev_adress, data, 1, 100);
}




// SPI

// DMA callbacks, when SPI finishes
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi)
{
	dma_flag = DMA_COMPLETED;
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi)
{
	dma_flag = DMA_COMPLETED;
}

void SPI_check_CTS(HAL_StatusTypeDef* status) {
	uint8_t attempt = 0;
	// Wait until Clear To Send (CTS) signal appears
	while (!(HAL_GPIO_ReadPin(CTS_PIN_GROUP, CTS_PIN_NUM)) && (attempt < 10)) {
		attempt++;
		HAL_Delay(100);
	}

	if (attempt == 10) {
		*status = 0x3; // Timeout
		return;
	}

	// Clear CTS once found, so it can function properly - use GET_INT_STATUS command with three zero bytes
	uint8_t data[] = {0x20, 0, 0, 0};
	*status |= HAL_SPI_Transmit(&hspi4, data, 4, 50);

	// SI is now ready to receive data
	// NOTE: response should be read, however, I have not found any description on interrupt mapping in received structure
}

void SPI_write(HAL_StatusTypeDef* status, uint8_t* data) {
	// Transmit data array
	SPI_check_CTS(status);
	*status |= HAL_SPI_Transmit(&hspi4, data, sizeof(data)/8, 50);
	return;
}

// SPI functions
void SPI_read(HAL_StatusTypeDef* status, uint8_t* data) {
	// Receive data array of given size defined by provided array
	// Maximum data length provided by SI chip is 16 bytes (-> max. 16 messages)
	// Repetitive reading (from 1st byte) is possible, but it has not been implemented yet


	SPI_check_CTS(status);

	// Duplicate data array with command and zeros (use static buffer)
	data_buffer[0] = data[0];

	// When reading data, don't forget to ignore the first byte
	*status |= HAL_SPI_TransmitReceive(&hspi4, data_buffer, data, sizeof(data)/8, 70);
}





void send_image(HAL_StatusTypeDef* status, uint8_t* data) {
	// Wait for previous DMA interrupt to occur, if not happened yet
	while(dma_flag == DMA_INPROGRESS) {HAL_Delay(10);}

	SPI_check_CTS(status);

	// Change DMA flag
	dma_flag = DMA_INPROGRESS;

	// Copy data (so we can work with data array)
	memcpy(data_buffer, data, 61);

	// Begin Tx, DMA fires an interrupt when done (see HAL_SPI_TxCpltCallback)
	*status = HAL_SPI_Transmit_DMA(&hspi4, data_buffer, 61);
}

void receive_image(HAL_StatusTypeDef* status, uint8_t* data) {
	// Wait for previous DMA interrupt to occur, if not happened yet
	while(dma_flag == DMA_INPROGRESS) {HAL_Delay(10);}

	SPI_check_CTS(status);

	// Change DMA flag
	dma_flag = DMA_INPROGRESS;

	// Load register address to temporary buffer
	data_buffer[0] = data[0];
	*status = HAL_SPI_TransmitReceive_DMA(&hspi4, data_buffer, data, 61);
	return;
}

