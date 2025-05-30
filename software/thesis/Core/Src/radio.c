#include "radio.h"

void radio_power_up(HAL_StatusTypeDef *status) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, SET);
	HAL_Delay(100);
	uint8_t data[] = {RF_POWER_UP};
	//uint8_t data[9] = {0x02, 0x01, 0x01, 0x01, 0xC9, 0xC3, 0x80, 0x44, 0xFF};
	uint8_t size = sizeof(data);
	uint16_t attempt = 0;
	//uint8_t NOP = 0x00;
	uint8_t *pData = data;
	SPI_HandleTypeDef *hspi = &hspi4;

#if defined (__GNUC__)
  __IO uint16_t *ptxdr_16bits = (__IO uint16_t *)(&(hspi->Instance->TXDR));
#endif /* __GNUC__ */

	// Optimization of HAL_SPI_Transmit

	if (hspi->State != HAL_SPI_STATE_READY) {
		*status = HAL_BUSY;
		return;
	}
	/* Lock the process */
	__HAL_LOCK(&hspi4);

	/* Set the transaction information */
	hspi->State = HAL_SPI_STATE_BUSY_TX;
	hspi->ErrorCode = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr = (const uint8_t*) pData;
	hspi->TxXferSize = size; // One byte
	hspi->TxXferCount = size;

	/*Init field not used in handle to zero */
	hspi->pRxBuffPtr = NULL;
	hspi->RxXferSize = (uint16_t) 0UL;
	hspi->RxXferCount = (uint16_t) 0UL;
	hspi->TxISR = NULL;
	hspi->RxISR = NULL;

	// Communication direction
	SPI_2LINES_TX(hspi);

	/* Set the number of data at current transfer */
	MODIFY_REG(hspi->Instance->CR2, SPI_CR2_TSIZE, size);


	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, RESET);

	// Wait for CTS to go high
	if (!(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9))) {
		// Wait until Clear To Send (CTS) signal appears
		while (!(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9))) {
			attempt++;
			if (attempt > 10000) {
				*status = 0x3; // Timeout
				// End process
				close_SPI(hspi);
				hspi->State = HAL_SPI_STATE_READY;

				/* Unlock the process */
				__HAL_UNLOCK(hspi);
				return;
			}
		}
	}
	/* Enable SPI peripheral */
	__HAL_SPI_ENABLE(hspi);
	/* Master transfer start */
	SET_BIT(hspi->Instance->CR1, SPI_CR1_CSTART);

	// Loop until the byte is sent
	// While this seems unnecessary, it covers 2 requirements - wait until SPI is ready to send and then send only one byte
	while (hspi->TxXferCount > 0UL) {
		/* Wait until TXP flag is set to send data */
		if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXP)) {
			if ((hspi->TxXferCount > 3UL)
					&& (hspi->Init.FifoThreshold > SPI_FIFO_THRESHOLD_03DATA)) {
				*((__IO uint32_t*) &hspi->Instance->TXDR) =
						*((const uint32_t*) hspi->pTxBuffPtr);
				hspi->pTxBuffPtr += sizeof(uint32_t);
				hspi->TxXferCount -= (uint16_t) 4UL;
			} else if ((hspi->TxXferCount > 1UL)
					&& (hspi->Init.FifoThreshold > SPI_FIFO_THRESHOLD_01DATA)) {
#if defined (__GNUC__)
				*ptxdr_16bits = *((const uint16_t*) hspi->pTxBuffPtr);
#else
		          *((__IO uint16_t *)&hspi->Instance->TXDR) = *((const uint16_t *)hspi->pTxBuffPtr);
		#endif /* __GNUC__ */
				hspi->pTxBuffPtr += sizeof(uint16_t);
				hspi->TxXferCount -= (uint16_t) 2UL;
			} else {
				*((__IO uint8_t*) &hspi->Instance->TXDR) =
						*((const uint8_t*) hspi->pTxBuffPtr);
				hspi->pTxBuffPtr += sizeof(uint8_t);
				hspi->TxXferCount--;
			}
		}
		/* Wait until TXP flag is set to send data */
		/*
		 if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXP)) {
		 *((__IO uint8_t*) &hspi->Instance->TXDR) =
		 *((const uint8_t*) hspi->pTxBuffPtr);
		 hspi->pTxBuffPtr += sizeof(uint8_t);
		 hspi->TxXferCount--;
		 }
		 */
	}

	// Wait until transfer is completed
	while (!(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_EOT))) {
	}

	// End process
	close_SPI(hspi);
	hspi->State = HAL_SPI_STATE_READY;

	/* Unlock the process */
	__HAL_UNLOCK(hspi);

	//*status = HAL_SPI_Transmit(&hspi4, &NOP, 1, 10);
	//*status = HAL_SPI_Transmit(&hspi4, data, size, 50);
	//SPI_write(status, data, &size);
}

void radio_pins(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_GPIO_PIN_CFG};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_TCXO(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_GLOBAL_XO_TUNE_2};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_global(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_GLOBAL_CONFIG_1};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_interrupts(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_INT_CTL_ENABLE_2};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_fast_registers(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_FRR_CTL_A_MODE_4};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_Tx(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PREAMBLE_TX_LENGTH_9};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_sync(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_SYNC_CONFIG_10};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_packet_CRC(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_CRC_CONFIG_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_Rx_thr(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_RX_THRESHOLD_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_Rx_CRC_1(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_FIELD_3_CRC_CONFIG_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_Rx_CRC_2(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_RX_FIELD_1_CRC_CONFIG_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_Rx_CRC_3(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_RX_FIELD_4_CRC_CONFIG_5};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_Rx_CRC_seed(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_CRC_SEED_31_24_4};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_mod(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_MOD_TYPE_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_freq_dev(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_FREQ_DEV_0_1};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_Tx_ramp(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_TX_RAMP_DELAY_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_NCO_offset(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_BCR_NCO_OFFSET_2_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_AFC(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_AFC_LIMITER_1_3};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_AGC_1(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_AGC_CONTROL_1};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_AGC_2(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_AGC_WINDOW_SIZE_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_RAW(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_RAW_CONTROL_10};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_search_period(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_RAW_SEARCH2_2};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_GFSK_thr(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_SPIKE_DET_2};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_RSSI_thr(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_RSSI_MUTE_1};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_RSSI_att(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_DSA_CTRL1_5};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_Rx_filter_1(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_Rx_filter_2(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_Rx_filter_3(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_PA_1(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PA_MODE_4};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_PA_2(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_SYNTH_PFDCP_CPFF_7};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_masks(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MATCH_VALUE_1_12};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_PLL(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_FREQ_CONTROL_INTE_8};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

// Modes
void radio_mode_Rx(HAL_StatusTypeDef* status) {
	// Enable Rx and then enter ready mode
	uint8_t data[] = {0x32, 0, 0, 0, 60, 0, 8, 8, 8};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_write_FIFO(HAL_StatusTypeDef* status, uint8_t* data, uint8_t* size) {
	(*size)++;
	uint8_t command[*size];
	command[0] = 0x66;
	for (uint8_t i = 1; i < *size; i++) {
		command[i] = data[i-1];
	}

	SPI_write(status, command, size);
}

void radio_mode_Tx(HAL_StatusTypeDef* status) {
	// Enable Tx mode. After transmission switch to Rx
	uint8_t data[] = {0x31, 0, 0x80, 0, 60, 0, 0};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_send_packet(HAL_StatusTypeDef* status, uint8_t* data, uint8_t* size) {
	// Fill FIFO
	radio_write_FIFO(status, data, size);
	// Begin transmission
	radio_mode_Tx(status);
}

void radio_read_fifo(HAL_StatusTypeDef* status, uint8_t* data) {
	uint8_t dummy_data[61];
	dummy_data[0] = 0x77;

	HAL_SPI_TransmitReceive(&hspi4, dummy_data, data, 61, 500);
}

uint8_t radio_read_PH_status(HAL_StatusTypeDef* status) {
	uint8_t data[2] = {0x21, 0xFF}; //Leave all interrupts pending or they will reset themselves
	uint8_t size = 2;
	SPI_read(status, data, &size, &size);

	uint8_t ph_status = data[0];
	/*
	if ((ph_status * 1 << 7) == (1 << 7)) {
		// Incoming packet matched the filter


	}
	*/
	return ph_status;
}

void radio_clear_PH_status(HAL_StatusTypeDef* status) {
	uint8_t data[2] = {0x21, 0x00}; //Leave all interrupts pending or they will reset themselves
	uint8_t size = 2;
	SPI_write(status, data, &size);
}

void radio_request_repetition(HAL_StatusTypeDef* status) {
	uint8_t size = 60;
	uint8_t data[] = {0};
	radio_send_packet(status, data, &size);
}
void radio_repetition_requested (HAL_StatusTypeDef* status) {
	radio_mode_Tx(status); // Reuse data in TX FIFO
}
