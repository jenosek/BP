#include "radio.h"

void radio_power_up(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_POWER_UP};
	SPI_write(status, data);
}

void radio_pins(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_GPIO_PIN_CFG};
	SPI_write(status, data);
}

void radio_TCXO(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_GLOBAL_XO_TUNE_2};
	SPI_write(status, data);
}

void radio_global(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_GLOBAL_CONFIG_1};
	SPI_write(status, data);
}

void radio_interrupts(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_INT_CTL_ENABLE_1};
	SPI_write(status, data);

void radio_fast_registers(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_FRR_CTL_A_MODE_4};
	SPI_write(status, data);
}

void radio_Tx(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PREAMBLE_TX_LENGTH_9};
	SPI_write(status, data);
}

void radio_sync(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_SYNC_CONFIG_10};
	SPI_write(status, data);
}

void radio_Tx_CRC(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_CRC_CONFIG_12};
	SPI_write(status, data);
}

void radio_Rx_thr(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_RX_THRESHOLD_12};
	SPI_write(status, data);
}

void radio_Rx_CRC_1(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_FIELD_3_CRC_CONFIG_12};
	SPI_write(status, data);
}

void radio_Rx_CRC_2(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_RX_FIELD_1_CRC_CONFIG_12};
	SPI_write(status, data);
}

void radio_Rx_CRC_3(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_RX_FIELD_4_CRC_CONFIG_5};
	SPI_write(status, data);
}

void radio_Rx_CRC_seed(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_PKT_CRC_SEED_31_24_4};
	SPI_write(status, data);
}

void radio_mod(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_MOD_TYPE_12};
	SPI_write(status, data);
}

void radio_freq_dev(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_FREQ_DEV_0_1};
	SPI_write(status, data);
}

void radio_Tx_ramp(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_TX_RAMP_DELAY_12};
	SPI_write(status, data);
}

void radio_NCO_offset(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_BCR_NCO_OFFSET_2_12};
	SPI_write(status, data);
}

void radio_AFC(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_AFC_LIMITER_1_3};
	SPI_write(status, data);
}

void radio_AGC_1(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_AGC_CONTROL_1};
	SPI_write(status, data);
}

void radio_AGC_2(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_AGC_WINDOW_SIZE_12};
	SPI_write(status, data);
}

void radio_RAW(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_AGC_WINDOW_SIZE_12};
	SPI_write(status, data);
}

void radio_search_period(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_RAW_CONTROL_10};
	SPI_write(status, data);
}

void radio_GFSK_thr(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_RAW_SEARCH2_2};
	SPI_write(status, data);
}

void radio_RSSI_thr(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_SPIKE_DET_2};
	SPI_write(status, data);
}

void radio_RSSI_att(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_RSSI_MUTE_1};
	SPI_write(status, data);
}

void radio_Rx_filter_1(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_DSA_CTRL1_5};
	SPI_write(status, data);
}

void radio_Rx_filter_2(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12};
	SPI_write(status, data);
}

void radio_PA_1(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_GLOBAL_XO_TUNE_2};
	SPI_write(status, data);
}

void radio_PA_2(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_GLOBAL_XO_TUNE_2};
	SPI_write(status, data);
}

void radio_masks(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_GLOBAL_XO_TUNE_2};
	SPI_write(status, data);
}

void radio_PLL(HAL_StatusTypeDef* status) {
	uint8_t data[] = {RF_GLOBAL_XO_TUNE_2};
	SPI_write(status, data);
}



