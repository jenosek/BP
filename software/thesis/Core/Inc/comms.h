#ifndef COMMS_H
#define COMMS_H


typedef enum
{
  CAMERA = 0x42,
  USB_C_CONTROLLER = 0b10000100,
} DeviceAdress;

static I2C_HandleTypeDef* p_I2C;
static SPI_HandleTypeDef* p_SPI;
static DMA_HandleTypeDef* p_SPI_DMA_Tx;
static DMA_HandleTypeDef* p_SPI_DMA_Rx;

void comms_init(I2C_HandleTypeDef* I2C_handle, SPI_HandleTypeDef* SPI_handle, DMA_HandleTypeDef* SPI_DMA_Tx, DMA_HandleTypeDef* SPI_DMA_Rx);
void I2C_write(HAL_StatusTypeDef* status, DeviceAdress dev_adress, uint8_t reg_adress, uint8_t* data);
void I2C_read(HAL_StatusTypeDef* status, DeviceAdress dev_adress, uint8_t reg_adress, uint8_t* data);

// SPI interrupt-based comms. (settings etc.)
void SPI_read(HAL_StatusTypeDef* status, uint8_t* data, uint8_t* length);
void SPI_write(HAL_StatusTypeDef* status, uint8_t* data);


// SPI DMA-based comms. (image data transfer)
void send_image(HAL_StatusTypeDef* status, uint8_t* data);
void receive_image(HAL_StatusTypeDef* status, uint8_t* data);
#endif
