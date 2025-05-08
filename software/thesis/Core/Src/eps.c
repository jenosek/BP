#include "eps.h"

char* GetDevID(HAL_StatusTypeDef* status) {
	char* IDs[3];
	uint8_t reg;
	uint8_t version;
	uint8_t product_ID;
	uint8_t revision_ID;

	// Read register
	*status |= I2C_read(USB_C_CONTROLLER, DEV_ID, &reg);

	// Filter data with use of mask
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

	return;
}

HAL_StatusTypeDef PowerReset() {
	HAL_StatusTypeDef status;
	uint8_t reg_val;

	// Reset all registers to default state
	status = I2C_read(USB_C_CONTROLLER, RESET, &reg_val);
	reg_val |= ((1<<1) | 1);
	status |= I2C_write(USB_C_CONTROLLER, RESET, reg_val);

	return status;
}


void PowerInit(HAL_StatusTypeDef* status) {
	uint8_t reg_val;

	// Reset all registers
	*status |= PowerReset();

	// Set device as SINK (Disable SRC/SNK toggle and poll CC lines only)
	// CONTROL2:(MODE, TOGGLE)
	*status |= I2C_read(USB_C_CONTROLLER, CONTROL2, &reg_val);
	reg_val |= ((1<<2) | 1);
	reg_val &= (0<<1);
	*status |= I2C_write(USB_C_CONTROLLER, CONTROL2, reg_val);


	// Initially, set current to USB default  (recommended by doc.)
	*status |= I2C_read(USB_C_CONTROLLER, CONTROL0, &reg_val);
	reg_val &= (0<<3);
	reg_val |= (1 << 2);
	*status = I2C_write(USB_C_CONTROLLER, CONTROL0, reg_val);

	// Disable current source(s) at CC lines
	*status |= I2C_read(USB_C_CONTROLLER, SWITCHES0, &reg_val);
	reg_val &= ((0 << 5) | (0 << 4));
	*status |= I2C_write(USB_C_CONTROLLER, SWITCHES0, reg_val);

	// Mask interrupt for current level request
	*status |= I2C_write(USB_C_CONTROLLER, MASK, 0xFE);

	// Mask interrupt for toggle status
	*status |= I2C_write(USB_C_CONTROLLER, MASKA, 0xBF);

	// Mask interrupt for GoodCRC acknowledge (BMC is not used)
	*status |= I2C_write(USB_C_CONTROLLER, MASKB, 0x01);

	return;
}

void PowerIncreaseCurrent(HAL_StatusTypeDef* status) {
	uint8_t reg_val;
	HAL_StatusTypeDef status;

	*status |= I2C_read(USB_C_CONTROLLER, CONTROL0, &reg_val);
	reg_val |= ((1<<3) | (1<<2));
	*status |= I2C_write(USB_C_CONTROLLER, CONTROL0, reg_val);

	return;
}


