#include "radio.h"

void radio_send_NOP(SPI_HandleTypeDef *hspi, uint32_t Timeout) {
	uint8_t NOP = 0x00;


	 //uint32_t tickstart;

	  /* Check Direction parameter */
	  //assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE_2LINES_TXONLY(hspi->Init.Direction));

	  /* Init tickstart for timeout management*/
	  //tickstart = HAL_GetTick();

	  if (hspi->State != HAL_SPI_STATE_READY)
	  {
	    return HAL_BUSY;
	  }


	  /* Lock the process */
	  __HAL_LOCK(hspi);

	  /* Set the transaction information */
	  hspi->State       = HAL_SPI_STATE_BUSY_TX;
	  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	  hspi->pTxBuffPtr  = (const uint8_t *)&NOP;
	  hspi->TxXferSize  = 1;
	  hspi->TxXferCount = 1;

	  /*Init field not used in handle to zero */
	  hspi->pRxBuffPtr  = NULL;
	  hspi->RxXferSize  = (uint16_t) 0UL;
	  hspi->RxXferCount = (uint16_t) 0UL;
	  hspi->TxISR       = NULL;
	  hspi->RxISR       = NULL;

	  /* Configure communication direction : 1Line */
	  /*
	  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
	  {
	    SPI_1LINE_TX(hspi);
	  }
	  else
	  {
	    SPI_2LINES_TX(hspi);
	  }
	*/
	  SPI_1LINE_TX(hspi);

	  /* Set the number of data at current transfer */
	  MODIFY_REG(hspi->Instance->CR2, SPI_CR2_TSIZE, 1);

	  /* Enable SPI peripheral */
	  __HAL_SPI_ENABLE(hspi);

	  if (hspi->Init.Mode == SPI_MODE_MASTER)
	  {
	    /* Master transfer start */
	    SET_BIT(hspi->Instance->CR1, SPI_CR1_CSTART);
	  }


	  /* Transmit data in 8 Bit mode */
	  while (hspi->TxXferCount > 0UL) {
	      /* Wait until TXP flag is set to send data */
	      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXP))
	      {
	        if ((hspi->TxXferCount > 3UL) && (hspi->Init.FifoThreshold > SPI_FIFO_THRESHOLD_03DATA))
	        {
	          *((__IO uint32_t *)&hspi->Instance->TXDR) = *((const uint32_t *)hspi->pTxBuffPtr);
	          hspi->pTxBuffPtr += sizeof(uint32_t);
	          hspi->TxXferCount -= (uint16_t)4UL;
	        }
	        else if ((hspi->TxXferCount > 1UL) && (hspi->Init.FifoThreshold > SPI_FIFO_THRESHOLD_01DATA))
	        {
	#if defined (__GNUC__)
	          *ptxdr_16bits = *((const uint16_t *)hspi->pTxBuffPtr);
	#else
	          *((__IO uint16_t *)&hspi->Instance->TXDR) = *((const uint16_t *)hspi->pTxBuffPtr);
	#endif /* __GNUC__ */
	          hspi->pTxBuffPtr += sizeof(uint16_t);
	          hspi->TxXferCount -= (uint16_t)2UL;
	        }
	        else
	        {
	          *((__IO uint8_t *)&hspi->Instance->TXDR) = *((const uint8_t *)hspi->pTxBuffPtr);
	          hspi->pTxBuffPtr += sizeof(uint8_t);
	          hspi->TxXferCount--;
	        }
	      }
	      else
	      {
	        /* Timeout management */
	        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
	        {
	          /* Call standard close procedure with error check */
	          SPI_CloseTransfer(hspi);

	          SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_TIMEOUT);
	          hspi->State = HAL_SPI_STATE_READY;

	          /* Unlock the process */
	          __HAL_UNLOCK(hspi);

	          return HAL_TIMEOUT;
	        }
	      }
	    }


	  /* Wait for Tx (and CRC) data to be sent */
	  if (SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_EOT, RESET, Timeout, tickstart) != HAL_OK)
	  {
	    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
	  }

	  /* Call standard close procedure with error check */
	  SPI_CloseTransfer(hspi);

	  hspi->State = HAL_SPI_STATE_READY;

	  /* Unlock the process */
	  __HAL_UNLOCK(hspi);

	  if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
	  {
	    *status |= HAL_ERROR;
	  }
}

void radio_power_up(HAL_StatusTypeDef* status) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, SET);
	uint8_t NOP = 0x00;
	uint8_t data[] = {RF_POWER_UP};
	uint8_t size = sizeof(data);
	uint8_t attempt = 0;
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, RESET);
	/*
	// Wait for CTS to go high
	if (!(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9))) {
		// Wait until Clear To Send (CTS) signal appears
		while (!(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9))) {
			attempt++;
			if (attempt > 30) {
				*status = 0x3; // Timeout
				return;
			}
		}
	}
	*/
	// Wait until Clear To Send (CTS) signal appears
			while (!(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9))) {
				attempt++;
				if (attempt > 30) {
					*status = 0x3; // Timeout
					return;
				}
			}

			// Check if the SPI is enabled
			  if((SPI4->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
			  {
			      // If disabled, I enable it
			      SET_BIT(SPI4->CR1, SPI_CR1_SPE);
			  }

			  while (!(SPI4->SR & SPI_SR_TXC));
			  // Send bytes over the SPI
			  LL_SPI_TransmitData8(SPI4,0x00);
			  // Wait until the transmission is complete
			  while (!(SPI4->SR & SPI_SR_TXC));

			  // Disable SPI
			  CLEAR_BIT(SPI4->CR1, SPI_CR1_SPE);

	//*status = HAL_SPI_Transmit(&hspi4, &NOP, 1, 10);
	*status = HAL_SPI_Transmit(&hspi4, data, size, 50);
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
	uint8_t data[] = {RF_INT_CTL_ENABLE_1};
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
	uint8_t data = {0x32, 0, 0, 0, 60, 0, 0, 8, 8};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}

void radio_mode_Tx(HAL_StatusTypeDef* status) {
	// Enable Tx mode. After transmission, stay in this mode
	uint8_t data = {0x31, 0, 7, 0, 60, 0, 0};
	uint8_t size = sizeof(data);
	SPI_write(status, data, &size);
}


