#ifndef INC_RADIO_H_
#define INC_RADIO_H_

#include "stm32h7xx_hal.h"
#include "radio_config_Si4468.h"
#include "comms.h"
#include "stm32h7xx_hal_def.h"




// Setup
void radio_power_up(HAL_StatusTypeDef* status);
void radio_pins(HAL_StatusTypeDef* status);
void radio_TCXO(HAL_StatusTypeDef* status);
void radio_global(HAL_StatusTypeDef* status);
void radio_interrupts(HAL_StatusTypeDef* status);
void radio_fast_registers(HAL_StatusTypeDef* status);
void radio_Tx(HAL_StatusTypeDef* status);
void radio_sync(HAL_StatusTypeDef* status);
void radio_packet_CRC(HAL_StatusTypeDef* status);
void radio_Rx_thr(HAL_StatusTypeDef* status);
void radio_Rx_CRC_1(HAL_StatusTypeDef* status);
void radio_Rx_CRC_2(HAL_StatusTypeDef* status);
void radio_Rx_CRC_3(HAL_StatusTypeDef* status);
void radio_Rx_CRC_seed(HAL_StatusTypeDef* status);
void radio_mod(HAL_StatusTypeDef* status);
void radio_freq_dev(HAL_StatusTypeDef* status);
void radio_Tx_ramp(HAL_StatusTypeDef* status);
void radio_NCO_offset(HAL_StatusTypeDef* status);
void radio_AFC(HAL_StatusTypeDef* status);
void radio_AGC_1(HAL_StatusTypeDef* status);
void radio_AGC_2(HAL_StatusTypeDef* status);
void radio_RAW(HAL_StatusTypeDef* status);
void radio_search_period(HAL_StatusTypeDef* status);
void radio_GFSK_thr(HAL_StatusTypeDef* status);
void radio_RSSI_thr(HAL_StatusTypeDef* status);
void radio_RSSI_att(HAL_StatusTypeDef* status);
void radio_Rx_filter_1(HAL_StatusTypeDef* status);
void radio_Rx_filter_2(HAL_StatusTypeDef* status);
void radio_Rx_filter_3(HAL_StatusTypeDef* status);
void radio_PA_1(HAL_StatusTypeDef* status);
void radio_PA_2(HAL_StatusTypeDef* status);
void radio_masks(HAL_StatusTypeDef* status);
void radio_PLL(HAL_StatusTypeDef* status);

// Modes
void radio_mode_Rx(HAL_StatusTypeDef* status);
void radio_write_FIFO(HAL_StatusTypeDef* status, uint8_t* data);
void radio_mode_Tx(HAL_StatusTypeDef* status);
void radio_send_packet(HAL_StatusTypeDef* status, uint8_t* data);
void radio_read_fifo(HAL_StatusTypeDef* status, uint8_t* data);

// NIRQ handling
uint8_t radio_read_PH_status(HAL_StatusTypeDef* status);
void radio_clear_PH_status(HAL_StatusTypeDef* status);
void radio_request_repetition(HAL_StatusTypeDef* status);
void radio_repetition_requested (HAL_StatusTypeDef* status);
void radio_send_ACK(HAL_StatusTypeDef* status);


#endif /* INC_RADIO_H_ */
