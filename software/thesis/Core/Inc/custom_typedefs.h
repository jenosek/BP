#ifndef CUSTOM_TYPEDEFS_H
#define CUSTOM_TYPEDEFS_H


// Common - error handling
typedef enum {
	OK,
	POWER_INIT,
	POWER_INC,
	RADIO_CTS,
	RADIO_PWUP,
	RADIO_SETUP,
	RADIO_SET_RX,
	RADIO_HW_INFO
}fault_flag;

// Setup - device adresses
typedef enum
{
  CAMERA = 0x42,
  USB_C_CONTROLLER = 0b01000100,
} DeviceAdress;



#endif /* INC_CUSTOM_TYPEDEFS_H_ */
