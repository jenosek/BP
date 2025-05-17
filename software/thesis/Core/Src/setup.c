#include "setup.h"

void setup(HAL_StatusTypeDef* status, fault_flag* error_index) {


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
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);

	/// Wait for CTS pin to go high
	SPI_check_CTS(status);
	if (*status != 0) {
		*error_index = RADIO_CTS;
		return;
	}

	/// Issue POWER_UP command - wake transceiver up

	if (*status != 0) {
		*error_index = RADIO_PWUP;
	}

	/// Pin config



}

