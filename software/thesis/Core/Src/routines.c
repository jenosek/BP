#include <routines.h>

void setup(HAL_StatusTypeDef* status, fault_flag* error_index) {

	/// Issue POWER_UP command - wake transceiver up
		radio_power_up(status);
		if (*status != 0) {
			*error_index = RADIO_PWUP;
			return;
		}

	// USB-C
	PowerInit(status);

	/// Check for errors, if non are found -> increase supply current
	if (*status == 0) {
		PowerIncreaseCurrent(status);
	}
	else {
		*error_index = POWER_INIT;
		return;
	}

	/// Check for error
	if (*status != 0) {
		*error_index = POWER_INC;
		return;
	}


	// Transceiver
	/// Pull down NSEL pin to begin SPI communication with transceiver
	//HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);

	/// Wait for CTS pin to go high
	/*

	*/


	/// Issue radio setup
	radio_pins(status);
	radio_TCXO(status);
	radio_global(status);
	radio_interrupts(status);
	radio_fast_registers(status);
	radio_Tx(status);
	radio_sync(status);
	radio_packet_CRC(status);
	radio_Rx_thr(status);
	radio_Rx_CRC_1(status);
	radio_Rx_CRC_2(status);
	radio_Rx_CRC_3(status);
	radio_Rx_CRC_seed(status);
	radio_mod(status);
	radio_freq_dev(status);
	radio_Tx_ramp(status);
	radio_NCO_offset(status);
	radio_AFC(status);
	radio_AGC_1(status);
	radio_AGC_2(status);
	radio_RAW(status);
	radio_search_period(status);
	radio_GFSK_thr(status);
	radio_RSSI_thr(status);
	radio_RSSI_att(status);
	radio_Rx_filter_1(status);
	radio_Rx_filter_2(status);
	radio_Rx_filter_3(status);
	radio_PA_1(status);
	radio_PA_2(status);
	radio_masks(status);
	radio_PLL(status);

	// Check for errors
	if (*status != 0) {
		*error_index = RADIO_SETUP;
		return;
	}

	// Init. radio into receive mode
	radio_mode_Rx(status);
	if (*status != 0) {
		*error_index = RADIO_SET_RX;
	}

}

void cmd_main_win() {
	uint8_t TxBuffer[] = "==================================================\r\n"
						 "Available ground station commands (type number):\r\n"
						 "--------------------------------------------------\r\n"
						 "1) Ping satellite \r\n"
						 "2) Get satellite's telemetry \r\n"
						 "3) Get image \r\n"
						 "4) Clear terminal \r\n"
						 "5) Ground station status \r\n"
						 "==================================================\r\n";
	uint16_t TxBufferLen = sizeof(TxBuffer);
	CDC_Transmit_HS(TxBuffer, TxBufferLen);
}

void cmd_img_options() {
	uint8_t TxBuffer[] = "==================================================\r\n"
					  	 "Select desired camera mode XY: \r\n"
						 "--------------------------------------------------\r\n"
						 "X - Resolution: \r\n"
						 "1) VGA (640x480)\r\n"
						 "2) CIF (352x288)\r\n"
						 "3) QVGA (320x240)\r\n"
						 "4) QCIF (176x144)\r\n"
						 "--------------------------------------------------\r\n"
						 "Y - Colour mode: \r\n"
						 "1) Black & White \r\n"
						 "2) Colourful \r\n"
						 "==================================================\r\n";
	uint16_t TxBufferLen = sizeof(TxBuffer);
	CDC_Transmit_HS(TxBuffer, TxBufferLen);
}

void radio_ping(HAL_StatusTypeDef* status) {
	// Fill 60byte packet with dummy data (all ones)
	uint8_t data[60];
	for (uint8_t i = 0; i < 60; i++) {
		data[i] = 0xFF;
	}
	radio_send_packet(status, data);
}
void get_img_res(HAL_StatusTypeDef* status, uint8_t* img_mode) {
	// Allocate sufficient memory space for requested image
		/// VGA = 640 * 480 * 2 = 614400
		/// CIF = 352 * 288 * 2 = 202752
		/// QVGA = 320 * 240 * 2 = 153600
		/// QCIF = 176 * 144 * 2 = 50688

			uint8_t res = (*img_mode & 0b11110000);
			switch (res) {
				case (0x00): {
					img_size = 614400;
					break;
				}
				case (0x10): {
					img_size = 202752;
					break;
				}
				case (0x20): {
					img_size = 153600;
					break;
				}
				case (0x30): {
					img_size = 50688;
					break;
				}
				default: {
					*status = HAL_ERROR;
					img_size = 0;
				}
			}
}
void request_img(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* img_mode) {

		get_img_res(status, img_mode);
		if (*status != 0) {
			*error_index = IMG_DEF;
		}

		// Realloc globally defined image buffer to required size
		uint8_t* prev_buff_loc = img_buffer;
		if (!((uint8_t*) realloc(img_buffer, img_size))) {
			free(prev_buff_loc);
			*status = HAL_ERROR;
			*error_index = MEM_REALLOC;
			return;
		}

		// Send request
		uint8_t packet_buffer[60];
		for (uint8_t i = 0; i < 60; i++) {
			packet_buffer[i] = 0xCC;
		}
		packet_buffer[1] = *img_mode;
		radio_send_packet(status, packet_buffer);
		img_flag = 1; // Activate flag
}

void capture_img(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* img_mode) {
	get_img_res(status, img_mode);

	// Realloc globally defined image buffer to required size
	uint8_t* prev_buff_loc = img_buffer;
	if (!((uint8_t*) realloc(img_buffer, img_size))) {
		free(prev_buff_loc);
		*status = HAL_ERROR;
		*error_index = MEM_REALLOC;
		return;
	}

	camera_init(status, img_mode);
	if (*status != 0) {
		*error_index = CAM_INIT;
		return;
	}
	camera_capture_photo(status, img_buffer, &img_size);

	if (*status != 0) {
		*error_index = CAM_CAPTURE;
		return;
	}
	img_flag = 1; // Set flag to indicate that image is stored in buffer and ready to be transmitted

}

void transmit_img(HAL_StatusTypeDef* status, fault_flag* error_index, uint32_t* index, uint32_t* size) {
	uint8_t packet[60];
	uint8_t prev_idex = *index;
	// Load data block to packet
	for (uint8_t i = 0; i < 60; i++) {
		if (*index >= *size) {
			img_flag = 0;
			*size = 0x100000000;
			break;
		}
		packet[i] = img_buffer[*index];
		(*index)++;
	}

	// Send packet
	radio_send_packet(status, &packet);
}

void nirq_handler(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* ping, uint8_t* ack, uint32_t* index) {

	// Packet handling
	uint8_t pending_interrupts = radio_read_PH_status(status);
	uint8_t reg = 0;
	uint8_t control = 0;

	reg = pending_interrupts & (1 << 4);
	control = (1 << 4);

	// Directly receiving
	if (reg == control) {
		// Read packet from FIFO
		uint8_t packet[61]; //Ignore the first byte
		uint8_t AAs = 0;
		uint8_t CCs = 0;
		uint8_t FFs = 0;
		uint8_t zeros = 0;

		radio_read_fifo(status, packet);
		if (*status != 0) {
			*status = HAL_ERROR;

		}


		for (uint8_t i = 1; i < 61; i++) {
			if (packet[i] == 0xAA) {
				AAs++;
			}
			else if (packet[i] == 0xCC) {
				CCs++;
			}
			else if (packet[i] == 0xFF) {
				FFs++;
			}
			else if (packet[i] == 0) {
				zeros++;
			}

		}

		// Data handling logic
		//--------------------------------------------------------------------------------------------------------
		/// PING - all bytes 0xFF

		/// Request for resend - all bytes 0x00

		/// Send ACK - all bytes 0xAA

		/// Request image (code numbers are in hexadecimal)
		//// 1st byte of 0xCC
		//// 2nd byte - resolution at the fist 4 bits (that includes MSB):
		///// 0x - VGA => allocate 614400 B
		///// 1x - CIF => allocate 202752 B
		///// 2x - QVGA => allocate 153600 B
		///// 3x - QCIF => allocate 50688 B
		//// ... colour scheme on 4 bits (side of LSB):
		///// x0 - B&W
		///// x1 - RGB565
		//// 3rd to 60th bytes are filled with 0xCC
		// NOTE: When B&W data are transmitted, expect only half of RF_FIFO usage due to erased chrominance
		/// After Tx receives ACK packet, transmission of 60 bytes of image data interleaved with ACKs begins
		//--------------------------------------------------------------------------------------------------------

		// PING
		if (FFs == 60) {
			if (*ping == 1) { // Response to ping received
				*ping = 2;

			} else {
				HAL_Delay(100);
				radio_ping(status); // Send ping response
			}
		}

		// Data retransmission request
		else if (zeros == 60) {
			radio_repetition_requested(status);
		}
		/// ACK
		else if (AAs == 60) {
			*ack = 1;
		}

		/// TELEMETRY

		/// IMG
		else if (CCs == 59) {
			uint8_t img_mode = packet[2];
			capture_img(status, error_index, &img_mode);

		}
		else if (img_flag) {
			for (uint8_t i = 1; i < 61; i++) {
				img_buffer[*index] = packet[i];
				(*index)++;
			}
		}

		// Return from function as data are about to be transmitted
		radio_clear_PH_status(status);
		return;
	}

	// Receiving process - transceiver is not using this for some reason
	// 1. detect packet header
	// 2. Control CRC
	// 3. Read RX_FIFO

	reg = pending_interrupts & (1 << 7);
	control = (1 << 7);


	if (reg == control) {
		// An incoming packet matched filter, check CRC
		reg = pending_interrupts & ((1 << 3) | (1 << 2));
		control = 0;
		if (reg == control) {
			// Read packet from FIFO
			uint8_t packet[60];
			uint8_t ones = 0;
			uint8_t zeros = 0;
			uint8_t rs= 60;
			uint8_t ts = 1;

			packet[0] = 0x77;
			SPI_read(status, packet, &ts, &rs);
			if (*status != 0) {
				*status = HAL_ERROR;

			}
			for (uint8_t i = 0; i < 60; i++) {
				if (packet[i] == 0xFF) {
					ones++;
				}
				else if (packet[i] == 0) {
					zeros++;
				}
			}

			// Data handling logic
			/// PING
			if (ones == 60) {
				if (*ping == 1) { // Response to ping received
					*ping = 2;

				}
				else {
					radio_ping(status); // Send ping response
				}
			}

			// Data retransmission request
			else if (zeros == 60) {
				radio_repetition_requested(status);
			}
			/// TELEMETRY

			/// IMG

			// Clear IRQ and wait for transmission
			radio_clear_PH_status(status);
			return;
		}

		// Request packet retransmission
		else {
			radio_request_repetition(status);
		}

	}
	/*
	reg = pending_interrupts & (1 << 6);
	control = 1 << 6;
	if (reg == control) {
		// Unrecognized packet, rearm RX
		radio_mode_Rx(status);

	}

	reg = pending_interrupts & (1 << 5);
	control = 1 << 5;
	if (reg == control) {
		// Buffer transmitted - switch to RX and listen
		radio_mode_Rx(status);
	}

	*/
	radio_clear_PH_status(status);
	radio_mode_Rx(status);



}

void get_GS_state(HAL_StatusTypeDef* status) {
	uint8_t rf_state[2] = {0x33, 0};
	uint8_t ts = 1;
	uint8_t rs = 2;

	SPI_read(status, rf_state, &ts, &rs);

	uint8_t main_state = rf_state[0] & 0b1111;
	uint8_t current_channel = rf_state[1];
	uint8_t state_buffer[24] = "                      \r\n";
	switch (main_state) {
		case 2: {
			strcpy(state_buffer, "State: SPI_ACTIVE");
			break;
		}
		case 3:
		case 4: {
			strcpy(state_buffer,"State: READY");
			break;
		}
		case 5: {
			strcpy(state_buffer,"State: TX_TUNE");
			break;
		}
		case 6: {
			strcpy(state_buffer,"State: RX_TUNE");
			break;
		}
		case 7: {
			strcpy(state_buffer,"State: TX");
			break;
		}
		case 8: {
			strcpy(state_buffer,"State: RX");
			break;
		}
		default: {
			strcpy(state_buffer,"State: not recognized");
		}
	}
	uint16_t state_buffer_len = sizeof(state_buffer);
	CDC_Transmit_HS(state_buffer, state_buffer_len);


	uint8_t ch_buff[] = "Current channel: ";
	state_buffer_len = sizeof(ch_buff);
	CDC_Transmit_HS(ch_buff, state_buffer_len);


	current_channel += (uint8_t) '0';
	CDC_Transmit_HS(&current_channel, 1);

	strcpy(ch_buff, "               \r\n");
	state_buffer_len = sizeof(ch_buff);
	CDC_Transmit_HS(ch_buff, state_buffer_len);

}
/*
char** get_radio_hw_info(HAL_StatusTypeDef* status, fault_flag* error_index) {
	// Returns char array with info about used transceiver
	// [0] = part number
	// [1] = chip revision
	// [2] = ROM ID


	uint8_t array[8] = {0};
	uint8_t tsize = 1;
	uint8_t rsize = 8;
	char info[3][4];

	uint16_t num = 0xFFFF;
	array[0] = 0x01;
	SPI_read(status, array, &tsize, &rsize);
	if (*status != 0) {
		*error_index = RADIO_HW_INFO;
	}

	num &= ((array[1] << 8) | array[2]);
	sprintf(info[0], "%04X", num);
	num = 0;
	num += array[0];
	sprintf(info[1], "%04X", num);

	num = 0;
	num += array[7];
	sprintf(info[2], "%04X", num);

	return info;
}
*/
