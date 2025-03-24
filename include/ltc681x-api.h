/*!
 * \file ltc681x-api.h
 * \brief This file contains the functions to communicate with a generic LTC681X chip
 *
 * \attention For SPI communication with the LTC, mode 3 is required (in CubeMX is CPOL="Low" and CPHA="2 edge")
 * and the MISO pin has to be set in Pull-Up mode (if it's not set on the hardware side)
 * Maximum SPI communication speed with the LTC is 1MHz
 *
 * \date 16 Dec 2023
 * \author Antonio Gelain [antonio.gelain2@gmail.com]
 */

#ifndef LTC681X_API_H
#define LTC681X_API_H

#include "ltc681x.h"

/*!
 * \brief Initialize the LTC681X chain structure
 *
 * \param chain The chain structure
 * \param ltc_count The total number of LTC681X in the chain
 */
void ltc681x_chain_init(Ltc681xChain * chain, size_t ltc_count);

/*!
 * \brief Check if the ADC conversion has ended or not
 *
 * \attention Do not pull the Chip Select(CS) high before the conversion has ended
 * otherwise this command will not work
 *
 * \param byte The single byte read after the pladc command
 * \return bool True if the conversion has ended, false otherwise
 */
bool ltc681x_pladc_check(uint8_t byte);

/*!****************************************/
/*           BROADCAST COMMANDS           */
/*!****************************************/

/*!
 * \brief Encode the configuration data of the LTC's to a broadcast write command
 * 
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_WRITE_BUFFER_SIZE macro to get the right size for the buffer
 * \attention The 'config' array size have to be equals to the number of LTC681X in the chain
 *
 * \param chain The LTC681X broadcast handler
 * \param config The configuration data
 * \param out The array where the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_wrcfg_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xCfgr * config,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast configuration read command
 * 
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_READ_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array where the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_rdcfg_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Decode the LTC broadcast configuration data
 *
 * \attention The 'out' array should be as large as the total number of LTC681X
 * in the chain
 *
 * \details The return value can be compared with the LTC681X_DATA_BUFFER_SIZE macro
 * to check if all bytes where decoded correctly
 *
 * \param chain The LTC681X broadcast handler
 * \param data The array of bytes to decode
 * \param out The array where the configuration data is stored
 * \return size_t The number of decoded bytes (PEC included)
 */
size_t ltc681x_rdcfg_decode_broadcast(
    Ltc681xChain * chain,
    uint8_t * data,
    Ltc681xCfgr * out
);
/*!
 * \brief Encode the LTC broadcast cell voltage read command
 *
 * \attention The 'out' array should be large enough to contain all the encode bytes
 * Use the LTC681X_READ_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param reg The register to read from
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_rdcv_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xCvxr reg,
    uint8_t * out
);
/*!
 * \brief Decode the LTC broadcast cell voltage data
 *
 * \attention The 'out' array should be large enough to store 'LTC681X_REG_CELL_COUNT'
 * voltages for each LTC681X in the chain
 *
 * \details The return value can be compared with the LTC681X_DATA_BUFFER_SIZE macro
 * to check if all bytes where decoded correctly
 *
 * \param chain The LTC681X broadcast handler
 * \param data The array of bytes to decode
 * \param out The array where the cell voltages are stored
 * \return size_t The number of decoded bytes (PEC included)
 */
size_t ltc681x_rdcv_decode_broadcast(
    Ltc681xChain * chain,
    uint8_t * data,
    uint16_t * out
);
/*!
 * \brief Encode the LTC broadcast auxiliary voltage read command
 *
 * \attention The 'out' array should be large enough to contain all the encode bytes
 * Use the LTC681X_READ_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param reg The register to read from
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_rdaux_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xAvxr reg,
    uint8_t * out
);
/*!
 * \brief Decode the LTC broadcast auxiliary voltage data
 *
 * \attention The 'out' array should be large enough to store 'LTC681X_REG_AUX_COUNT'
 * voltages for each LTC681X in the chain
 *
 * \details The return value can be compared with the LTC681X_DATA_BUFFER_SIZE macro
 * to check if all bytes where decoded correctly
 *
 * \param chain The LTC681X broadcast handler
 * \param data The array of bytes to decode
 * \param out The array where the auxiliary voltages are stored
 * \return size_t The number of decoded bytes (PEC included)
 */
size_t ltc681x_rdaux_decode_broadcast(
    Ltc681xChain * chain,
    uint8_t * data,
    uint16_t * out
);
/*!
 * \brief Encode the LTC broadcast status read command
 * 
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_READ_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param reg The register to read from
 * \param out The array where the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_rdstat_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xStxr reg,
    uint8_t * out
);
/*!
 * \brief Decode the LTC broadcast status data
 *
 * \attention The 'out' array should be as large as the total number of LTC681X
 * in the chain
 *
 * \details The return value can be compared with the LTC681X_DATA_BUFFER_SIZE macro
 * to check if all bytes where decoded correctly
 *
 * \param chain The LTC681X broadcast handler
 * \param reg The register where the data comes from
 * \param data The array of bytes to decode
 * \param out The array where the status data is stored
 * \return size_t The number of decoded bytes (PEC included)
 */
size_t ltc681x_rdstat_decode_broadcast(
    Ltc681xChain * chain,
    Ltc681xStxr reg,
    uint8_t * data,
    Ltc681xStr * out
);
/*!
 * \brief Encode S pin control data of the LTC's into a broadcast write command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_WRITE_BUFFER_SIZE macro to get the right size for the buffer
 * \attention The 'data' array size have to be equal to the total number of
 * S pin control values of all the LTC's in the chain (LTC681X_SCTL_COUNT * chain.count)
 *
 * \param chain The LTC681X broadcast handler
 * \param data The array of data to write
 * \param out The array where the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_wrsctrl_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * data,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast S pin control read command
 *
 * \attention The 'out' array should be large enough to contain all the encode bytes
 * Use the LTC681X_READ_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_rdsctrl_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Decode the LTC broadcast S pin control data
 *
 * \attention The 'out' array should be large enough to store 'LTC681X_SCTL_COUNT'
 * values for each LTC681X in the chain
 *
 * \details The return value can be compared with the LTC681X_DATA_BUFFER_SIZE macro
 * to check if all bytes where decoded correctly
 *
 * \param chain The LTC681X broadcast handler
 * \param data The array of bytes to decode
 * \param out The array where the S pin control data is stored
 * \return size_t The number of decoded bytes (PEC included)
 */
size_t ltc681x_rdsctrl_decode_broadcast(
    Ltc681xChain * chain,
    uint8_t * data,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast start S pin control pulsing and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array where the encoded bytes are written
 * \return The number of encoded bytes
 */
size_t ltc681x_stsctrl_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broacast clear S pin control register command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array where the encoded bytes are written
 * \return The number of encoded bytes
 */
size_t ltc681x_clrsctrl_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Encode PWM data of the LTC's into a broadcast write command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_WRITE_BUFFER_SIZE macro to get the right size for the buffer
 * \attention The 'data' array size have to be equal to the total number of
 * PWM values of all the LTC's in the chain (LTC681X_PWM_COUNT * chain.count)
 *
 * \param chain The LTC681X broadcast handler
 * \param data The array of data to write
 * \param out The array where the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_wrpwm_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * data,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast pwm read command
 *
 * \attention The 'out' array should be large enough to contain all the encode bytes
 * Use the LTC681X_READ_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_rdpwm_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Decode the LTC broadcast pwm data
 *
 * \attention The 'out' array should be large enough to store 'LTC681X_PWM_COUNT'
 * values for each LTC681X in the chain
 *
 * \details The return value can be compared with the LTC681X_DATA_BUFFER_SIZE macro
 * to check if all bytes where decoded correctly
 *
 * \param chain The LTC681X broadcast handler
 * \param data The array of bytes to decode
 * \param out The array where the pwm data is stored
 * \return size_t The number of decoded bytes (PEC included)
 */
size_t ltc681x_rdpwm_decode_broadcast(
    Ltc681xChain * chain,
    uint8_t * data,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast cells ADC conversion start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param dcp Allow (or not) measurement during discharge
 * \param cells The selected cells
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_adcv_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xDcp dcp,
    Ltc681xCh cells,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast open wire ADC conversion start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param pup Pull-up/Pull-down option
 * \param dcp Allow (or not) measurement during discharge
 * \param cells The selected cells
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_adow_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xPup pup,
    Ltc681xDcp dcp,
    Ltc681xCh cells,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast cell voltage conversion self test start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param test_mode Self test mode option
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_cvst_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xSt test_mode,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast cell 7 voltage overlap measurement start command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param dcp Allow (or not) measurement during discharge
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_adol_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xDcp dcp,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast GPIOs ADC conversion start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param gpios The selected GPIOs
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_adax_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xChg gpios,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast GPIOs ADC conversion with digital redundancy start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param gpios The selected GPIOs
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_adaxd_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xChg gpios,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast GPIOs conversion self test start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param test_mode Self test mode option
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_axst_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xSt test_mode,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast status group ADC conversion start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param groups The selected status group
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_adstat_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xChst groups,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast status group ADC conversion with digital redundancy start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param groups The selected status group
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_adstatd_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xChst groups,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast status group conversion self test start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param test_mode Self test mode option
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_statst_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xSt test_mode,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast cell voltage and GPIOs ADC conversion start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param dcp Allow (or not) measurement during discharge
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_adcvax_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xDcp dcp,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast cell voltage and SC conversion start and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param mode The ADC conversion mode
 * \param dcp Allow (or not) measurement during discharge
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_adcvsc_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xMd mode,
    Ltc681xDcp dcp,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast clear cell voltage register command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_clrcell_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast clear GPIOs voltage register command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_clraux_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast clear status register command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_clrstat_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast poll ADC conversion status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \details To check for the ADC conversion status send the encoded command and
 * then read a single byte and check the status using the 'ltc681x_pladc_check' function
 * until it has ended conversion
 *
 * \attention Do not pull the Chip Select(CS) high before the conversion has ended
 * otherwise this command will not work
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_pladc_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Encode the LTC broadcast diagnose MUX and poll status command
 *
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_POLL_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array were the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_diagn_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Encode the external communication data of the LTC's to a broadcast write command
 * 
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_WRITE_BUFFER_SIZE macro to get the right size for the buffer
 * \attention The 'comm' array size have to be equals to the number of LTC681X in the chain
 *
 * \param chain The LTC681X broadcast handler
 * \param comm The communication data
 * \param out The array where the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_wrcomm_encode_broadcast(
    Ltc681xChain * chain,
    Ltc681xComm * comm,
    uint8_t * out
);
/*!
 * \brief Encode the external communication data of the LTC's broadcast read command
 * 
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_READ_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array where the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_rdcomm_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);
/*!
 * \brief Decode the LTC broadcast external communication data
 *
 * \attention The 'out' array should be as large as the total number of LTC681X
 * in the chain
 *
 * \details The return value can be compared with the LTC681X_DATA_BUFFER_SIZE macro
 * to check if all bytes where decoded correctly
 *
 * \param chain The LTC681X broadcast handler
 * \param data The array of bytes to decode
 * \param out The array where the communication data is stored
 * \return size_t The number of decoded bytes (PEC included)
 */
size_t ltc681x_rdcomm_decode_broadcast(
    Ltc681xChain * chain,
    uint8_t * data,
    Ltc681xComm * out
);
/*!
 * \brief Encode the external communication broadcast start command
 * 
 * \attention The 'out' array should be large enough to contain all the encoded bytes
 * Use the LTC681X_STCOMM_BUFFER_SIZE macro to get the right size for the buffer
 *
 * \param chain The LTC681X broadcast handler
 * \param out The array where the encoded bytes are written
 * \return size_t The number of encoded bytes
 */
size_t ltc681x_stcomm_encode_broadcast(
    Ltc681xChain * chain,
    uint8_t * out
);


/*!****************************************/
/*            ADDRESS COMMANDS            */
/*!****************************************/

#endif  // LTC681X_API_H

