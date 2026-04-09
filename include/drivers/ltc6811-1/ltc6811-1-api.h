/*!
 * \file            ltc6811-1-api.h
 * \date            2026-03-10
 * \authors         Antonio Gelain [antonio.gelain2@gmail.com]
 *
 * \brief           Function declarations needed for the LTC6811-1.
 *
 * \details         For additional information refer to the datasheet:
 * \link            https://www.analog.com/media/en/technical-documentation/data-sheets/ltc6811-1-6811-2.pdf
 *
 * \warning         For SPI communication with the LTC6811-1, mode 3 is
 *                  required (in CubeMX is CPOL="Low" and CPHA="2 edge") and
 *                  the MISO pin has to be set in Pull-Up mode.
 *                  Maximum SPI communication speed with the LTC is 1MHz.
 */

#ifndef LTC6811_1_API_H
#define LTC6811_1_API_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "ltc6811-1.h"

/*!
 * \brief           Initialize the IC handler structure
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       ltc_count: The total number of ICs in the chain
 */
void ltc6811_1_init(struct Ltc68111Handler *handler, size_t ltc_count);

/*!
 * \brief           Check if the ADC conversion has ended or not
 *
 * \warning         Do not pull the Chip Select(CS) high before the conversion
 *                  has ended otherwise this command will not work
 *
 * \param[in]       byte: A byte read after the pladc command was sent
 * \return          True if the conversion has ended, false otherwise
 */
bool ltc6811_1_pladc_is_completed(const uint8_t byte);

/*!
 * \brief           Encode the configuration data of the ICs into a broadcast
 *                  write command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_WRITE_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \warning         The \c config array size MUST be equal to the number of ICs
 *                  in the chain
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       config: The configuration data array
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_WRITE_BUFFER_SIZE
 */
size_t ltc6811_1_wrcfg_encode_broadcast(
    const struct Ltc68111Handler *handler,
    struct Ltc68111Cfgr *config,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast configuration read command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_READ_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_READ_BUFFER_SIZE
 */
size_t ltc6811_1_rdcfg_encode_broadcast(const struct Ltc68111Handler *handler, uint8_t *out);

/*!
 * \brief           Decode the ICs broadcast read command response payload into
 *                  configuration data
 *
 * \warning         The \c out array should be as large as the total number of
 *                  ICs in the chain
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       payload: The array of bytes to decode
 * \param[out]      out: The array where the configuration data is stored
 * \return          The number of decoded bytes (PEC included), should be equal to \c LTC6811_1_DATA_BUFFER_SIZE
 */
size_t ltc6811_1_rdcfg_decode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *payload,
    struct Ltc68111Cfgr *out);

/*!
 * \brief           Encode the broadcast cell voltage read command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encode bytes.
 *                  Use the \c LTC6811_1_READ_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       reg: The register to read from
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_READ_BUFFER_SIZE
 */
size_t ltc6811_1_rdcv_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Cvxr reg,
    uint8_t *out);

/*!
 * \brief           Decode the ICs broadcast read command response payload into
 *                  cell voltages
 *
 * \warning         The \c out array should be large enough to store
 *                  \c LTC6811_1_REG_CELL_COUNT voltages for each IC in the
 *                  chain
 *
 * \details         Cell voltages are 16-bit values represented in mV * 10
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       payload: The array of bytes to decode
 * \param[out]      out: The array where the cell voltages are stored
 * \return          The number of decoded bytes (PEC included), should be equal to \c LTC6811_1_DATA_BUFFER_SIZE
 */
size_t ltc6811_1_rdcv_decode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *payload,
    uint16_t *out);

/*!
 * \brief           Encode the broadcast auxiliary voltage read command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encode bytes.
 *                  Use the \c LTC6811_1_READ_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       reg: The register to read from
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_READ_BUFFER_SIZE
 */
size_t ltc6811_1_rdaux_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Avxr reg,
    uint8_t *out);

/*!
 * \brief           Decode the ICs broadcast read command response payload into
 *                  auxiliary voltages
 *
 * \warning         The \c out array should be large enough to store
 *                  \c LTC6811_1_REG_AUX_COUNT voltages for each IC in the chain
 *
 * \details         Auxiliary voltages are 16-bit values represented in mV * 10
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       payload: The array of bytes to decode
 * \param[out]      out: The array where the auxiliary voltages are stored
 * \return          The number of decoded bytes (PEC included), should be equal to \c LTC6811_1_DATA_BUFFER_SIZE
 */
size_t ltc6811_1_rdaux_decode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *payload,
    uint16_t *out);

/*!
 * \brief           Encode the broadcast status read command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_READ_BUFFER_SIZE macro to get the correct
 *                  size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       reg: The register to read from
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_READ_BUFFER_SIZE
 */
size_t ltc6811_1_rdstat_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Stxr reg,
    uint8_t *out);

/*!
 * \brief           Decode the ICs broadcast read command response payload into
 *                  status data
 *
 * \warning         The \c out array should be as large as the total number of
 *                  ICs in the chain
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       reg: The register where the data comes from
 * \param[in]       payload: The array of bytes to decode
 * \param[out]      out: The array where the status data is stored
 * \return          The number of decoded bytes (PEC included), should be equal to \c LTC6811_1_DATA_BUFFER_SIZE
 */
size_t ltc6811_1_rdstat_decode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Stxr reg,
    uint8_t *payload,
    struct Ltc68111Str *out);

/*!
 * \brief           Encode S pin control data of the ICs into a broadcast
 *                  write command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_WRITE_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \warning         The \c data array size MUST be equal to the number of S pin
 *                  control values of all the ICs in the chain
 *                  (i.e. \c LTC6811_1_SCTRL_COUNT * \c handler.count)
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       payload: The array of data to write
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_WRITE_BUFFER_SIZE
 */
size_t ltc6811_1_wrsctrl_encode_broadcast(
    const struct Ltc68111Handler *handler,
    const uint8_t *payload,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast S pin control read command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encode bytes.
 *                  Use the \c LTC6811_1_READ_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_DATA_BUFFER_SIZE
 */
size_t ltc6811_1_rdsctrl_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

/*!
 * \brief           Decode the ICs broadcast read command response payload into
 *                  S pin control data
 *
 * \warning         The \c out array should be large enough to store
 *                  \c LTC6811_1_SCTL_COUNT values for each IC in the chain
 *
 * \details         S pin control bits are represented as 4 bits values that
 *                  controls the behaviors of the S pins
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       payload: The array of bytes to decode
 * \param[out]      out: The array where the S pin control data is stored
 * \return          The number of decoded bytes (PEC included), should be equal to \c LTC6811_1_DATA_BUFFER_SIZE
 */
size_t ltc6811_1_rdsctrl_decode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *payload,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast start S pin control pulsing and poll
 *                  status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_stsctrl_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

/*!
 * \brief           Encode the broacast clear S pin control register command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_clrsctrl_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

/*!
 * \brief           Encode PWM data of the ICs into a broadcast write command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_WRITE_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \warning         The \c data array size MUST be equal to the total number of
 *                  PWM values of all the ICs in the chain
 *                  (i.e. \c LTC6811_1_PWM_COUNT * \c chain.count)
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       payload: The array of data to write
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_WRITE_BUFFER_SIZE
 */
size_t ltc6811_1_wrpwm_encode_broadcast(
    const struct Ltc68111Handler *handler,
    const uint8_t *payload,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast PWM read command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encode bytes.
 *                  Use the \c LTC6811_1_READ_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_READ_BUFFER_SIZE
 */
size_t ltc6811_1_rdpwm_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

/*!
 * \brief           Decode the ICs broadcast read command response payload into
 *                  PWM data
 *
 * \warning         The \c out array should be large enough to store
 *                  \c LTC6811_1_PWM_COUNT values for each IC in the chain
 *
 * \details         PWM values are 4-bit integers that represent a specific
 *                  duty cycle from 0 = 0% to 15 = 100%
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       payload: The array of bytes to decode
 * \param[out]      out: The array where the pwm data is stored
 * \return          The number of decoded bytes (PEC included), should be equal to \c LTC6811_1_DATA_BUFFER_SIZE
 */
size_t ltc6811_1_rdpwm_decode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *payload,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast cells ADC conversion start and poll
 *                  status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       dcp: Allow (or not) measurements during discharge
 * \param[in]       cells: The selected cells to measure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_adcv_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Dcp dcp,
    enum Ltc68111Ch cells,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast Open Wire ADC conversion start and
 *                  poll status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       pup: Set Pull-up or Pull-down current
 * \param[in]       dcp: Allow (or not) measurement during discharge
 * \param[in]       cells: The selected cells to measure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_adow_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Pup pup,
    enum Ltc68111Dcp dcp,
    enum Ltc68111Ch cells,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast cell voltage conversion self test
 *                  start and poll status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       test_mode: Self test mode option
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_cvst_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111St test_mode,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast cell 7 voltage overlap measurement
 *                  start command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       dcp: Allow (or not) measurement during discharge
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_adol_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Dcp dcp,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast GPIOs ADC conversion start and poll
 *                  status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the correct
 *                  size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       gpios: The selected GPIOs
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_adax_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Chg gpios,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast GPIOs ADC conversion with digital
 *                  redundancy start and poll status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       gpios: The selected GPIOs
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_adaxd_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Chg gpios,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast GPIOs conversion self test start and
 *                  poll status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       test_mode: Self test mode option
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_axst_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111St test_mode,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast status group ADC conversion start and
 *                  poll status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       group: The selected status group
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_adstat_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Chst group,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast status group ADC conversion with
 *                  digital redundancy start and poll status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       groups: The selected status group
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_adstatd_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Chst groups,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast status group conversion self test
 *                  start and poll status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       test_mode: Self test mode option
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_statst_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111St test_mode,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast cell voltage and GPIOs ADC conversion
 *                  start and poll status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       dcp: Allow (or not) measurement during discharge
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_adcvax_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Dcp dcp,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast cell voltage and SC conversion start
 *                  and poll status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       mode: The selected ADC conversion mode
 * \param[in]       dcp: Allow (or not) measurement during discharge
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_adcvsc_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Dcp dcp,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast clear cell voltage register command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_clrcell_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast clear GPIOs voltage register command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_clraux_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast clear status register command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_clrstat_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast poll ADC conversion status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \warning         Do not pull the Chip Select(CS) high before the conversion
 *                  has ended otherwise this command will not work
 *
 * \details         To check if the ADC conversion has ended, after sending
 *                  the encoded command, read a single byte and check its status
 *                  using the \c ltc6811_1_pladc_is_completed function until it has
 *                  ended conversion
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_pladc_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

/*!
 * \brief           Encode the broadcast diagnose MUX and poll status command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_POLL_BUFFER_SIZE macro to get the
 *                  right size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_POLL_BUFFER_SIZE
 */
size_t ltc6811_1_diagn_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

/*!
 * \brief           Encode the external communication data of the ICs into a
 *                  broadcast write command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_WRITE_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \warning         The \c comm array size MUST be equals to the number of ICs
 *                  in the chain
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       comms: The communication data to write
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_WRITE_BUFFER_SIZE
 */
size_t ltc6811_1_wrcomm_encode_broadcast(
    const struct Ltc68111Handler *handler,
    struct Ltc68111Comm *comms,
    uint8_t *out);

/*!
 * \brief           Encode the external communication data of the ICs into a
 *                  broadcast read command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_READ_BUFFER_SIZE macro to get the correct
 *                  size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_READ_BUFFER_SIZE
 */
size_t ltc6811_1_rdcomm_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

/*!
 * \brief           Decode the ICs broadcast external communication payload
 *
 * \warning         The \c out array should be as large as the total number of
 *                  ICs in the chain
 *
 * \param[in]       handler: The IC handler structure
 * \param[in]       payload: The array of bytes to decode
 * \param[out]      out: The array where the communication data is stored
 * \return          The number of decoded bytes (PEC included), should be equal to \c LTC6811_1_DATA_BUFFER_SIZE
 */
size_t ltc6811_1_rdcomm_decode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *payload,
    struct Ltc68111Comm *out);

/*!
 * \brief           Encode the external communication broadcast start command
 *
 * \warning         The \c out array should be large enough to contain all the
 *                  encoded bytes.
 *                  Use the \c LTC6811_1_STCOMM_BUFFER_SIZE macro to get the
 *                  correct size for the buffer
 *
 * \param[in]       handler: The IC handler structure
 * \param[out]      out: The array where the encoded bytes are written
 * \return          The number of encoded bytes, should be equal to \c LTC6811_1_STCOMM_BUFFER_SIZE
 */
size_t ltc6811_1_stcomm_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out);

#endif // LTC6811_1_API_H
