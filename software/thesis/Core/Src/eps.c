#include "eps.h"

char* GetDevID() {
	char* IDs[3];
	HAL_StatusTypeDef status;
	uint8_t reg;
	uint8_t version;
	uint8_t product_ID;
	uint8_t revision_ID;

	// Read register
	status = read(USB_C_CONTROLLER, DEV_ID, &reg);

	// Filter data with use of masks
	version = reg & 0b11110000;
	product_ID = reg & ((1 << 3) | (1 << 2));
	revision_ID = reg & ((1 << 1) | 1);


	// Translate binary to char array
	//// Save letter of revision from ASCII
	IDs[0] = version + 57;
	IDs[3] = revision_ID + 65;

	//// Find case of product
	switch (product_ID) {
	case 0:
		IDs[1] = "FUSB302BMPX";
	case 1:
		IDs[1] = "FUSB302B01MPX";
	case 2:
		IDs[1] = "FUSB302B10MPX";
	case 3:
		IDs[1] = "FUSB302B11MPX";
	}
	default:
		IDs[1] = "Error";
}


