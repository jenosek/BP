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
	RADIO_HW_INFO,
	IMG_DEF,
	CAM_INIT,
	CAM_CAPTURE,
	RADIO_FIFO_READ
}fault_flag;

// Setup - device adresses
typedef enum
{
  CAMERA = 0x42,
  USB_C_CONTROLLER = 0b01000100,
} DeviceAdress;

// CAMERA MODES ALIASES
typedef enum
{
  CAM_VGA = 0x00,
  CAM_CIF = 0x10,
  CAM_QVGA = 0x20,
  CAM_QCIF = 0x30
} CAM_resolution;

typedef enum
{
  CAM_YUV = 0x00,
  CAM_RGB = 0x01

} CAM_format;

#endif /* INC_CUSTOM_TYPEDEFS_H_ */
