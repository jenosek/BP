#include "eps.h"

char* GetDevID(HAL_StatusTypeDef* status) {
	char IDs[3][13];
	uint8_t reg;
	uint8_t version;
	uint8_t product_ID;
	uint8_t revision_ID;
	char temp;

	// Read register
	I2C_read(status, USB_C_CONTROLLER, DEV_ID, &reg);

	// Filter data with use of mask
	version = (reg & 0b11110000) + 57;
	product_ID = reg & ((1 << 3) | (1 << 2));
	revision_ID = (reg & ((1 << 1) | 1)) + 65;


	// Translate binary to char array
	//// Save letter of revision from ASCII
	temp = (char)version;
	strcpy(IDs[0], temp);
	strcpy(IDs[3], revision_ID);

	//// Find case of product
	switch (product_ID) {
	case 0:
		temp = "FUSB302BMPX";
	case 1:
		temp = "FUSB302B01MPX";
	case 2:
		temp = "FUSB302B10MPX";
	case 3:
		temp = "FUSB302B11MPX";
	default:
		temp = "Error";
	}
	strcpy(IDs[1], temp);
	return;
}

void PowerReset(HAL_StatusTypeDef* status) {
	uint8_t reg_val;

	// Reset all registers to default state
	I2C_read(status, USB_C_CONTROLLER, RESET, &reg_val);
	reg_val++;
	I2C_write(status, USB_C_CONTROLLER, RESET, &reg_val);
}


void PowerInit(HAL_StatusTypeDef* status) {
	uint8_t reg_val;

	// Reset all registers
	PowerReset(status);

	// Set device as SINK (Disable SRC/SNK toggle and poll CC lines only)
	// CONTROL2:(MODE, TOGGLE)
	I2C_read(status, USB_C_CONTROLLER, CONTROL2, &reg_val);
	reg_val |= ((1<<2) | 1);
	reg_val &= ~(1<<1);
	I2C_write(status, USB_C_CONTROLLER, CONTROL2, &reg_val);


	// Initially, set current to USB default  (recommended by doc.)
	I2C_read(status, USB_C_CONTROLLER, CONTROL0, &reg_val);
	reg_val &= ~(1<<3);
	reg_val |= (1 << 2);
	I2C_write(status, USB_C_CONTROLLER, CONTROL0, &reg_val);

	// Disable current source(s) at CC lines
	I2C_read(status, USB_C_CONTROLLER, SWITCHES0, &reg_val);
	reg_val &= ~((1 << 5) | (1 << 4));
	I2C_write(status, USB_C_CONTROLLER, SWITCHES0, &reg_val);

	// Mask interrupt for current level request
	reg_val = 0xFE;
	I2C_write(status, USB_C_CONTROLLER, MASK, &reg_val);

	// Mask interrupt for toggle status
	reg_val = 0xBF;
	I2C_write(status, USB_C_CONTROLLER, MASKA, &reg_val);

	// Mask interrupt for GoodCRC acknowledge (BMC is not used)
	reg_val = 0x01;
	I2C_write(status, USB_C_CONTROLLER, MASKB, &reg_val);
}

void PowerIncreaseCurrent(HAL_StatusTypeDef* status) {
	uint8_t reg_val;

	I2C_read(status, USB_C_CONTROLLER, CONTROL0, &reg_val);
	reg_val |= ((1<<3) | (1<<2));
	I2C_write(status, USB_C_CONTROLLER, CONTROL0, &reg_val);

	return;
}


