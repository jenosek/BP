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
						 "1) Get satellite's telemetry \r\n "
						 "2) Get image \r\n "
						 "3) Clear terminal \r\n"
						 "==================================================\r\n";
	uint16_t TxBufferLen = sizeof(TxBuffer);
	CDC_Transmit_HS(TxBuffer, TxBufferLen);
}

void cmd_img_options() {
	uint8_t TxBuffer[] = "==================================================\r\n"
					  	 "Select desired camera mode XY: \r\n"
						 "--------------------------------------------------\r\n"
						 "X - Resolution: \r\n"
						 "1) VGA (640x480)"
						 "2) CIF (352x288)"
						 "3) QVGA (320x240)"
						 "4) QCIF (176x144)"
						 "--------------------------------------------------\r\n"
						 "Y - Colour mode: \r\n"
						 "1) Black & White \r\n"
						 "2) Colourful \r\n"
						 "==================================================\r\n";
	uint16_t TxBufferLen = sizeof(TxBuffer);
	CDC_Transmit_HS(TxBuffer, TxBufferLen);
}

void capture_img(HAL_StatusTypeDef* status, fault_flag* error_index, uint8_t* img_mode) {
	/*#define IMG_SIZE 614400 // VGA = 640 * 480 * 2
//#define IMG_SIZE 202752 // CIF = 352 * 288 * 2
//#define IMG_SIZE 153600 // QVGA = 320 * 240 * 2
//#define IMG_SIZE 50688 // QCIF = 176 * 144 * 2*/

	uint8_t res = (*img_mode & 0b11110000);
	uint32_t size;
	switch (res) {
		case (0x00): {
			size = 614400;
			break;
		}
		case (0x10): {
			size = 202752;
			break;
		}
		case (0x20): {
			size = 153600;
			break;
		}
		case (0x30): {
			size = 50688;
			break;
		}
		default: {
			*status = HAL_ERROR;
			*error_index = IMG_DEF;
			return;
		}
	}

	uint8_t buff[size];
	camera_init(status, img_mode);
	if (*status != 0) {
		*error_index = CAM_INIT;
	}
	camera_capture_photo(status, buff, &size);

	if (*status != 0) {
		*error_index = CAM_CAPTURE;
	}
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
