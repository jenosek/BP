#include "setup.h"

void setup(HAL_StatusTypeDef* status, uint8_t* error_index) {
	// USB-C
	PowerInit(HAL_StatusTypeDef* status);

	/// Check for errors, if non are found -> increase supply current
	if (*status == 0) {
		PowerIncreaseCurrent(HAL_StatusTypeDef* status);
	}
	else {
		*error_index = 1;
		return;
	}

	/// Check for error
	if (*status != 0) {
		*error_index = 2;
		return;
	}


	// Transceiver




}

