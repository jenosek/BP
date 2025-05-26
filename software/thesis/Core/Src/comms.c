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
void close_SPI(SPI_HandleTypeDef* hspi) {
	uint32_t itflag = hspi->Instance->SR;

	  __HAL_SPI_CLEAR_EOTFLAG(hspi);
	  __HAL_SPI_CLEAR_TXTFFLAG(hspi);

	  /* Disable SPI peripheral */
	  __HAL_SPI_DISABLE(hspi);

	  /* Disable ITs */
	  __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_EOT | SPI_IT_TXP | SPI_IT_RXP | SPI_IT_DXP | SPI_IT_UDR | SPI_IT_OVR | \
	                              SPI_IT_FRE | SPI_IT_MODF));

	  /* Disable Tx DMA Request */
	  CLEAR_BIT(hspi->Instance->CFG1, SPI_CFG1_TXDMAEN | SPI_CFG1_RXDMAEN);

	  /* Report UnderRun error for non RX Only communication */
	  if (hspi->State != HAL_SPI_STATE_BUSY_RX)
	  {
	    if ((itflag & SPI_FLAG_UDR) != 0UL)
	    {
	      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_UDR);
	      __HAL_SPI_CLEAR_UDRFLAG(hspi);
	    }
	  }

	  /* Report OverRun error for non TX Only communication */
	  if (hspi->State != HAL_SPI_STATE_BUSY_TX)
	  {
	    if ((itflag & SPI_FLAG_OVR) != 0UL)
	    {
	      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_OVR);
	      __HAL_SPI_CLEAR_OVRFLAG(hspi);
	    }

	#if (USE_SPI_CRC != 0UL)
	    /* Check if CRC error occurred */
	    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
	    {
	      if ((itflag & SPI_FLAG_CRCERR) != 0UL)
	      {
	        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);
	        __HAL_SPI_CLEAR_CRCERRFLAG(hspi);
	      }
	    }
	#endif /* USE_SPI_CRC */
	  }

	  /* SPI Mode Fault error interrupt occurred -------------------------------*/
	  if ((itflag & SPI_FLAG_MODF) != 0UL)
	  {
	    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_MODF);
	    __HAL_SPI_CLEAR_MODFFLAG(hspi);
	  }

	  /* SPI Frame error interrupt occurred ------------------------------------*/
	  if ((itflag & SPI_FLAG_FRE) != 0UL)
	  {
	    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FRE);
	    __HAL_SPI_CLEAR_FREFLAG(hspi);
	  }

	  hspi->TxXferCount = (uint16_t)0UL;
	  hspi->RxXferCount = (uint16_t)0UL;
}

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
	//uint8_t data[] = {0x20, 0, 0, 0}; //GET_INT_STATUS to clear CTS
	// Wait until Clear To Send (CTS) signal appears
	while (!(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9))) {
		attempt++;
		if (attempt > 100) {
			*status = 0x3; // Timeout
			return;
		}
		HAL_Delay(100);
	}

	//*status |= HAL_SPI_Transmit(&hspi4, data, 4, 100);

	// SI is now ready to receive data

}

void SPI_write(HAL_StatusTypeDef* status, uint8_t* data, uint8_t* size) {
	// Transmit data array
	SPI_check_CTS(status);
	if (*status != 0) {return;}
	*status |= HAL_SPI_Transmit(&hspi4, data, *size, 100);
}

// SPI functions
void SPI_read(HAL_StatusTypeDef* status, uint8_t* data, uint8_t* TxSize, uint8_t* RxSize) {
	// Receive data array of given RxSize (useful data only, padding is being added here)
	// Maximum data length provided by SI chip is 16 bytes (-> max. 16 messages)
	// Repetitive reading is possible, but it has not been implemented yet
	/*
	for (uint8_t i = 0; i < *TxSize; i++) {
		data_buffer[i] = data[i];
	}
	*/

	SPI_check_CTS(status);
	if (*status != 0) {return;}
	/*
	// Duplicate data array with command and zeros (use static buffer)
	data_buffer[0] = data[0];
	*/

	// Address of requested register
	*status |= HAL_SPI_Transmit(&hspi4, data, *TxSize, 70);

	// Wait for CTS
	SPI_check_CTS(status);
	if (*status != 0) {return;}


	// Once approved, prepare for reading (READ_CMD_BUFF)
	data[0] = 0x44;
	data[1] = 0xFF;

	// Insert padding (Tx delay + CTS byte)
	*RxSize += 2;

	// Issue reading command
	*status |= HAL_SPI_TransmitReceive(&hspi4, data, data_buffer, *RxSize, 100);


	// Load read data into data array and remove padding at the beginning
	for (uint8_t i = 2; i < *RxSize; i++) {
		data[i-2] = data_buffer[i];
	}

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

	// DON'T FORGET TO IGNORE THE FIRST BYTE
	return;
}

