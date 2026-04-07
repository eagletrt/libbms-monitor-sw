/*!
 * \file            test-ltc6811-1.h
 * \date            2025-03-08
 * \authors         Antonio Gelain [antonio.gelain2@gmail.com]
 *
 * \brief           Tests for the LTC6811-1 drivers
 */

#include "eagletrt-api.h"
#include "unity.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

#include "ltc6811-1.h"
#include "ltc6811-1-api.h"

uint16_t cmd_pecs[] = {
    [LTC6811_1_CMD_WRCFGA] = 0x3d6e,
    [LTC6811_1_CMD_WRCFGB] = 0xb19e,
    [LTC6811_1_CMD_RDCFGA] = 0x2b0a,
    [LTC6811_1_CMD_RDCFGB] = 0x2cc8,
    [LTC6811_1_CMD_RDCVA] = 0x07c2,
    [LTC6811_1_CMD_RDCVB] = 0x9a94,
    [LTC6811_1_CMD_RDCVC] = 0x5e52,
    [LTC6811_1_CMD_RDCVD] = 0xc304,
    [LTC6811_1_CMD_RDCVE] = 0xd560,
    [LTC6811_1_CMD_RDCVF] = 0x4836,
    [LTC6811_1_CMD_RDAUXA] = 0xefcc,
    [LTC6811_1_CMD_RDAUXB] = 0x729a,
    [LTC6811_1_CMD_RDAUXC] = 0x64fe,
    [LTC6811_1_CMD_RDAUXD] = 0xf9a8,
    [LTC6811_1_CMD_RDSTATA] = 0xed72,
    [LTC6811_1_CMD_RDSTATB] = 0x7024,
    [LTC6811_1_CMD_WRSCTRL] = 0x5cec,
    [LTC6811_1_CMD_WRPWM] = 0x0000,
    [LTC6811_1_CMD_WRPSB] = 0xb4e2,
    [LTC6811_1_CMD_RDSCTRL] = 0xc1ba,
    [LTC6811_1_CMD_RDPWM] = 0x9d56,
    [LTC6811_1_CMD_RDPSB] = 0x29b4,
    [LTC6811_1_CMD_STSCTRL] = 0x8e4e,
    [LTC6811_1_CMD_CLRSCTRL] = 0x057c,
    [LTC6811_1_CMD_ADCV] = 0x7c20,
    [LTC6811_1_CMD_ADOW] = 0x73a4,
    [LTC6811_1_CMD_CVST] = 0x8a0c,
    [LTC6811_1_CMD_ADOL] = 0xa6c4,
    [LTC6811_1_CMD_ADAX] = 0x5bec,
    [LTC6811_1_CMD_ADAXD] = 0x0a3a,
    [LTC6811_1_CMD_AXST] = 0xadc0,
    [LTC6811_1_CMD_ADSTAT] = 0xb3e2,
    [LTC6811_1_CMD_ADSTATD] = 0xe234,
    [LTC6811_1_CMD_STATST] = 0x45ce,
    [LTC6811_1_CMD_ADCVAX] = 0x1418,
    [LTC6811_1_CMD_ADCVSC] = 0xfc16,
    [LTC6811_1_CMD_CLRCELL] = 0xc9c0,
    [LTC6811_1_CMD_CLRAUX] = 0xdfa4,
    [LTC6811_1_CMD_CLRSTAT] = 0x5496,
    [LTC6811_1_CMD_PLADC] = 0xf36c,
    [LTC6811_1_CMD_DIAGN] = 0x785e,
    [LTC6811_1_CMD_WRCOMM] = 0x24b2,
    [LTC6811_1_CMD_RDCOMM] = 0x32d6,
    [LTC6811_1_CMD_STCOMM] = 0xb9e4
};

/*!
 * \brief               Declaration of internal functions. Needed to unit test
 */
uint16_t prv_ltc6811_1_pec15(uint8_t *data, size_t len);
size_t prv_ltc6811_1_pec_calc(uint8_t *payload, size_t len);
bool prv_ltc6811_1_pec_is_correct(uint8_t *data, size_t len);
size_t prv_ltc6811_1_cmd_encode(enum Ltc68111Command cmd, uint8_t *out);
enum Ltc68111Command prv_ltc6811_1_cmd_set_md(enum Ltc68111Command cmd, enum Ltc68111Md mode);
enum Ltc68111Command prv_ltc6811_1_cmd_set_pup(enum Ltc68111Command cmd, enum Ltc68111Pup pup);
enum Ltc68111Command prv_ltc6811_1_cmd_set_st(enum Ltc68111Command cmd, enum Ltc68111St mode);
enum Ltc68111Command prv_ltc6811_1_cmd_set_dcp(enum Ltc68111Command cmd, enum Ltc68111Dcp dcp);
enum Ltc68111Command prv_ltc6811_1_cmd_set_ch(enum Ltc68111Command cmd, enum Ltc68111Ch cells);
enum Ltc68111Command prv_ltc6811_1_cmd_set_chg(enum Ltc68111Command cmd, enum Ltc68111Chg gpios);
enum Ltc68111Command prv_ltc6811_1_cmd_set_chst(enum Ltc68111Command cmd, enum Ltc68111Chst groups);

#define LTC6811_1_COUNT (3U)
struct Ltc68111Handler ltc6811_1;

void setUp(void) {
    ltc6811_1_init(&ltc6811_1, LTC6811_1_COUNT);
}

void tearDown(void) {
}

/*!
 * \defgroup            enum Test for enum definitions
 * @{
 */

void test_md_enum_fields_equivalency(void) {
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_MD_422HZ, LTC6811_1_MD_1KHZ, "Mode 422Hz differs from mode 1kHz");
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_MD_27KHZ, LTC6811_1_MD_14KHZ, "Mode 27kHz differs from mode 14kHz");
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_MD_7KHZ, LTC6811_1_MD_3KHZ, "Mode 7kHz differs from mode 3kHz");
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_MD_26HZ, LTC6811_1_MD_2KHZ, "Mode 26Hz differs from mode 2kHz");
}

void test_ch_enum_fields_equivalency(void) {
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_CH_7, LTC6811_1_CH_1, "Channel 7 differs from channel 1");
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_CH_8, LTC6811_1_CH_2, "Channel 8 differs from channel 2");
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_CH_9, LTC6811_1_CH_3, "Channel 9 differs from channel 3");
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_CH_10, LTC6811_1_CH_4, "Channel 10 differs from channel 4");
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_CH_11, LTC6811_1_CH_5, "Channel 11 differs from channel 5");
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_CH_12, LTC6811_1_CH_6, "Channel 12 differs from channel 6");
}

/*! @} */

/*!
 * \defgroup            struct Test for struct definitions
 * @{
 */

void test_cfgr_struct_size_in_bytes(void) {
    const size_t cfgr_expected_size = 6U; /*!< Configuration register size in bytes */
    TEST_ASSERT_EQUAL_size_t(cfgr_expected_size, sizeof(struct Ltc68111Cfgr));
}

void test_str_struct_size_in_bytes(void) {
    const size_t str_expected_size = 6U * LTC6811_1_STXR_COUNT; /*!< Status register size in bytes */
    TEST_ASSERT_EQUAL_size_t(str_expected_size, sizeof(struct Ltc68111Str));
}

void test_comm_struct_size_in_bytes(void) {
    const size_t comm_expected_size = 6U * LTC6811_1_COMMX_COUNT; /*!< COMM register size in bytes */
    TEST_ASSERT_EQUAL_size_t(comm_expected_size, sizeof(struct Ltc68111Comm));
}

/*! @} */

/*!
 * \defgroup            pec Test for Packet Error Code calculation
 * @{
 */

void test_pec_calculation_with_valid_payload(void) {
    uint8_t payload[] = { 0x01, 0x20, 0x34, 0xaa, 0xff, 0x00 };
    size_t payload_length = sizeof(payload) / sizeof(payload[0]);
    uint16_t expected_pec = 0x4eaa;
    uint16_t pec = prv_ltc6811_1_pec15(payload, payload_length);
    TEST_ASSERT_EQUAL_UINT16(expected_pec, pec);
}

void test_pec_calculation_with_null_payload(void) {
    TEST_IGNORE_MESSAGE("Payload asserted to be not NULL");
}

void test_pec_calculation_with_zero_length(void) {
    uint8_t payload[] = { 0x00 };
    size_t payload_length = 0;
    uint16_t expected_pec = 0x20; // 32
    uint16_t pec = prv_ltc6811_1_pec15(payload, payload_length);
    TEST_ASSERT_EQUAL_UINT16(expected_pec, pec);
}

void test_pec_calculation_and_join_with_valid_payload(void) {
#define PAYLOAD_LENGTH (6 + LTC6811_1_PEC_BYTE_COUNT)
    uint8_t payload[PAYLOAD_LENGTH] = { 0xf5, 0x15, 0x46, 0x99, 0xe2, 0x15 };
    size_t payload_length = PAYLOAD_LENGTH - LTC6811_1_PEC_BYTE_COUNT;
    uint8_t expected_payload[PAYLOAD_LENGTH] = { 0xf5, 0x15, 0x46, 0x99, 0xe2, 0x15, 0x12, 0x1e };
    size_t byte_count = prv_ltc6811_1_pec_calc(payload, payload_length);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(PAYLOAD_LENGTH, byte_count, "Payload byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_payload, payload, PAYLOAD_LENGTH, "Payload data do not match");
#undef PAYLOAD_LENGTH
}

void test_pec_calculation_and_join_with_null_payload(void) {
    TEST_IGNORE_MESSAGE("Payload asserted to be not NULL");
}

void test_pec_calculation_and_join_with_zero_length(void) {
    uint8_t payload[LTC6811_1_PEC_BYTE_COUNT] = { 0 };
    size_t payload_length = 0;
    uint8_t expected_payload[LTC6811_1_PEC_BYTE_COUNT] = { 0x00, 0x20 };
    size_t byte_count = prv_ltc6811_1_pec_calc(payload, payload_length);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(LTC6811_1_PEC_BYTE_COUNT, byte_count, "Payload byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_payload, payload, LTC6811_1_PEC_BYTE_COUNT, "Payload data do not match");
}

void test_pec_is_correct_with_matching_pec(void) {
#define PAYLOAD_LENGTH (6 + LTC6811_1_PEC_BYTE_COUNT)
    uint8_t payload[PAYLOAD_LENGTH] = { 0xf5, 0x15, 0x46, 0x99, 0xe2, 0x15, 0x12, 0x1e };
    TEST_ASSERT_TRUE(prv_ltc6811_1_pec_is_correct(payload, PAYLOAD_LENGTH));
#undef PAYLOAD_LENGTH
}

void test_pec_is_correct_with_non_matching_pec(void) {
#define PAYLOAD_LENGTH (6 + LTC6811_1_PEC_BYTE_COUNT)
    uint8_t payload[PAYLOAD_LENGTH] = { 0x01, 0x20, 0x34, 0xaa, 0xff, 0x00, 0x69, 0x69 };
    TEST_ASSERT_FALSE(prv_ltc6811_1_pec_is_correct(payload, PAYLOAD_LENGTH));
#undef PAYLOAD_LENGTH
}

void test_pec_is_correct_with_null_payload(void) {
    TEST_IGNORE_MESSAGE("Payload asserted to be not NULL");
}

void test_pec_is_correct_with_length_less_than_two(void) {
    TEST_IGNORE_MESSAGE("Payload size asserted to be greater than or equal to two");
}

/*! @} */

/*!
 * \defgroup            command Test for command creation
 * @{
 */

void test_cmd_encode_with_valid_command(void) {
#define BUFFER_LENGTH (LTC6811_1_CMD_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT)
    uint8_t expected[] = {
        [0] = (LTC6811_1_CMD_CVST >> 8) & 0x07,
        [1] = LTC6811_1_CMD_CVST & 0xff,
        [2] = (cmd_pecs[LTC6811_1_CMD_CVST] >> 8) & 0xff,
        [3] = cmd_pecs[LTC6811_1_CMD_CVST] & 0xff
    };
    uint8_t result[BUFFER_LENGTH] = { 0 };
    size_t byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_CVST, result);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(BUFFER_LENGTH, byte_count, "Payload byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Payload data do not match");
#undef BUFFER_LENGTH
}

void test_cmd_set_md_with_invalid_mode(void) {
    enum Ltc68111Command expected_cmd = LTC6811_1_CMD_ADCV;
    enum Ltc68111Command cmd = prv_ltc6811_1_cmd_set_md(LTC6811_1_CMD_ADCV, LTC6811_1_MD_COUNT);
    TEST_ASSERT_EQUAL_HEX(expected_cmd, cmd);
}

void test_cmd_set_pup_with_invalid_pullup_pulldown(void) {
    enum Ltc68111Command expected_cmd = LTC6811_1_CMD_ADOW;
    enum Ltc68111Command cmd = prv_ltc6811_1_cmd_set_pup(LTC6811_1_CMD_ADOW, LTC6811_1_PUP_COUNT);
    TEST_ASSERT_EQUAL_HEX(expected_cmd, cmd);
}

void test_cmd_set_st_with_invalid_mode(void) {
    enum Ltc68111Command expected_cmd = LTC6811_1_CMD_CVST;
    enum Ltc68111Command cmd = prv_ltc6811_1_cmd_set_st(LTC6811_1_CMD_CVST, LTC6811_1_ST_COUNT);
    TEST_ASSERT_EQUAL_HEX(expected_cmd, cmd);
}

void test_cmd_set_dcp_with_invalid_discharge_option(void) {
    enum Ltc68111Command expected_cmd = LTC6811_1_CMD_ADOL;
    enum Ltc68111Command cmd = prv_ltc6811_1_cmd_set_dcp(LTC6811_1_CMD_ADOL, LTC6811_1_DCP_COUNT);
    TEST_ASSERT_EQUAL_HEX(expected_cmd, cmd);
}

void test_cmd_set_ch_with_invalid_cell(void) {
    enum Ltc68111Command expected_cmd = LTC6811_1_CMD_ADCV;
    enum Ltc68111Command cmd = prv_ltc6811_1_cmd_set_ch(LTC6811_1_CMD_ADCV, LTC6811_1_CH_COUNT);
    TEST_ASSERT_EQUAL_HEX(expected_cmd, cmd);
}

void test_cmd_set_chg_with_invalid_gpio(void) {
    enum Ltc68111Command expected_cmd = LTC6811_1_CMD_ADAX;
    enum Ltc68111Command cmd = prv_ltc6811_1_cmd_set_chg(LTC6811_1_CMD_ADAX, LTC6811_1_CHG_COUNT);
    TEST_ASSERT_EQUAL_HEX(expected_cmd, cmd);
}

void test_cmd_set_chst_with_invalid_status(void) {
    enum Ltc68111Command expected_cmd = LTC6811_1_CMD_ADSTAT;
    enum Ltc68111Command cmd = prv_ltc6811_1_cmd_set_chst(LTC6811_1_CMD_ADSTAT, LTC6811_1_CHST_COUNT);
    TEST_ASSERT_EQUAL_HEX(expected_cmd, cmd);
}

/*! @} */

/*!
 * \defgroup            init Test for initialization
 * @{
 */

void test_init_with_valid_parameters(void) {
    ltc6811_1_init(&ltc6811_1, LTC6811_1_COUNT);
    TEST_ASSERT_EQUAL_size_t(LTC6811_1_COUNT, ltc6811_1.count);
}

void test_init_with_null_handler(void) {
    ltc6811_1_init(NULL, LTC6811_1_COUNT);
    TEST_PASS();
}

void test_init_with_zero_count(void) {
    ltc6811_1_init(&ltc6811_1, 0);
    TEST_ASSERT_GREATER_THAN_size_t(0, ltc6811_1.count);
}

/*! @} */

/*!
 * \defgroup            wrcfg Test the write configuration command
 * @{
 */

void test_wrcfg_encode_with_valid_configuration(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Cfgr config[LTC6811_1_COUNT] = { 0 };
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };

    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        config[ltc].ADCOPT = 0b0;
        config[ltc].DTEN = 0b1;
        config[ltc].REFON = 0b0;
        config[ltc].GPIO = 0b10101;
        config[ltc].VUV = 0b101001010101;
        config[ltc].VOV = 0b010101011010;
        config[ltc].DCC = 0b010110101010;
        config[ltc].DCTO = 0b0101;

        const size_t cmd_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_WRCFGA, expected);
        const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
        const size_t reverse_index = cmd_byte_count +
                                     (LTC6811_1_COUNT - ltc - 1U) * payload_byte_count;
        expected[reverse_index + 0] = 0b10101010;
        expected[reverse_index + 1] = 0b01010101;
        expected[reverse_index + 2] = 0b10101010;
        expected[reverse_index + 3] = 0b01010101;
        expected[reverse_index + 4] = 0b10101010;
        expected[reverse_index + 5] = 0b01010101;
        const size_t _ = prv_ltc6811_1_pec_calc(expected + reverse_index, LTC6811_1_REG_BYTE_COUNT);
        EAGLETRT_API_UNUSED(_);
    }

    const size_t encoded_byte_count = ltc6811_1_wrcfg_encode_broadcast(&ltc6811_1, config, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(BUFFER_LENGTH, encoded_byte_count, "Encoded bytes count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrcfg_encode_encoded_payload_reverse_order_with_valid_configuration(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Cfgr config[LTC6811_1_COUNT] = { 0 };
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };

    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        config[ltc].DCTO = ltc;

        const size_t cmd_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_WRCFGA, expected);
        const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
        const size_t reverse_index = cmd_byte_count +
                                     (LTC6811_1_COUNT - ltc - 1U) * payload_byte_count;
        expected[reverse_index + 5] = ltc << 4;
        const size_t _ = prv_ltc6811_1_pec_calc(expected + reverse_index, LTC6811_1_REG_BYTE_COUNT);
        EAGLETRT_API_UNUSED(_);
    }

    const size_t encoded_byte_count = ltc6811_1_wrcfg_encode_broadcast(&ltc6811_1, config, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(BUFFER_LENGTH, encoded_byte_count, "Encoded bytes count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrcfg_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Cfgr config[LTC6811_1_COUNT] = { 0 };
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrcfg_encode_broadcast(NULL, config, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrcfg_encode_with_null_configuration(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrcfg_encode_broadcast(&ltc6811_1, NULL, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrcfg_encode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Cfgr config[LTC6811_1_COUNT] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrcfg_encode_broadcast(&ltc6811_1, config, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            rdcfg Test the read configuration command
 * @{
 */

void test_rdcfg_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_READ_BUFFER_SIZE)
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };
    size_t encoded_byte_count = ltc6811_1_rdcfg_encode_broadcast(NULL, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_rdcfg_encode_with_null_output_array(void) {
    size_t encoded_byte_count = ltc6811_1_rdcfg_encode_broadcast(&ltc6811_1, NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

void test_rdcfg_decode_with_valid_payload(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Cfgr expected_config[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Cfgr config[LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        expected_config[ltc].ADCOPT = 0b0;
        expected_config[ltc].DTEN = 0b1;
        expected_config[ltc].REFON = 0b0;
        expected_config[ltc].GPIO = 0b10101;
        expected_config[ltc].VUV = 0b101001010101;
        expected_config[ltc].VOV = 0b010101011010;
        expected_config[ltc].DCC = 0b010110101010;
        expected_config[ltc].DCTO = 0b0101;

        payload[expected_byte_count + 0] = 0b10101010;
        payload[expected_byte_count + 1] = 0b01010101;
        payload[expected_byte_count + 2] = 0b10101010;
        payload[expected_byte_count + 3] = 0b01010101;
        payload[expected_byte_count + 4] = 0b10101010;
        payload[expected_byte_count + 5] = 0b01010101;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    const size_t decoded_byte_count = ltc6811_1_rdcfg_decode_broadcast(&ltc6811_1, payload, config);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_config, config, sizeof(config[0]), LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdcfg_decode_decoded_payload_same_order_with_valid_configuration(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Cfgr expected_config[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Cfgr config[LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        expected_config[ltc].DCTO = ltc;

        payload[expected_byte_count + 5] = ltc << 4U;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    const size_t decoded_byte_count = ltc6811_1_rdcfg_decode_broadcast(&ltc6811_1, payload, config);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_config, config, sizeof(config[0]), LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdcfg_decode_with_invalid_pec(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Cfgr expected_config[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Cfgr config[LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        if (ltc > 0) {
            expected_config[ltc].ADCOPT = 0b0;
            expected_config[ltc].DTEN = 0b1;
            expected_config[ltc].REFON = 0b0;
            expected_config[ltc].GPIO = 0b10101;
            expected_config[ltc].VUV = 0b101001010101;
            expected_config[ltc].VOV = 0b010101011010;
            expected_config[ltc].DCC = 0b010110101010;
            expected_config[ltc].DCTO = 0b0101;
        }

        payload[expected_byte_count + 0] = 0b10101010;
        payload[expected_byte_count + 1] = 0b01010101;
        payload[expected_byte_count + 2] = 0b10101010;
        payload[expected_byte_count + 3] = 0b01010101;
        payload[expected_byte_count + 4] = 0b10101010;
        payload[expected_byte_count + 5] = 0b01010101;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }
    // Set PEC of the first IC to an invalid value
    const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    payload[6] = payload[7] = 0;

    const size_t decoded_byte_count = ltc6811_1_rdcfg_decode_broadcast(&ltc6811_1, payload, config);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count - payload_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(&expected_config[0], &config[0], sizeof(config[0]), "Payload with wrong PEC was decoded");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(&expected_config[1], &config[1], sizeof(config[1]), LTC6811_1_COUNT - 1, "Payload with correct PEC was not decoded");
#undef BUFFER_LENGTH
}

void test_rdcfg_decode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Cfgr expected_config[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Cfgr config[LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdcfg_decode_broadcast(NULL, payload, config);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_config, config, sizeof(config[0]), LTC6811_1_COUNT, "Decoded configuration has changed");
#undef BUFFER_LENGTH
}

void test_rdcfg_decode_with_null_payload(void) {
    struct Ltc68111Cfgr expected_config[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Cfgr config[LTC6811_1_COUNT] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdcfg_decode_broadcast(&ltc6811_1, NULL, config);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_config, config, sizeof(config[0]), LTC6811_1_COUNT, "Decoded configuration has changed");
}

void test_rdcfg_decode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdcfg_decode_broadcast(&ltc6811_1, payload, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            rdcv Test the read voltage command
 * @{
 */

void test_rdcv_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_READ_BUFFER_SIZE)
    uint8_t expected_payload[BUFFER_LENGTH] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t byte_count = ltc6811_1_rdcv_encode_broadcast(NULL, LTC6811_1_CVAR, payload);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0, byte_count, "Encoded byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_payload, payload, BUFFER_LENGTH, "Encoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdcv_encode_with_null_output_array(void) {
    size_t byte_count = ltc6811_1_rdcv_encode_broadcast(&ltc6811_1, LTC6811_1_CVAR, NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0, byte_count, "Decoded byte count do not match");
}

void test_rdcv_encode_with_invalid_register(void) {
#define BUFFER_LENGTH (LTC6811_1_READ_BUFFER_SIZE)
    uint8_t expected_payload[BUFFER_LENGTH] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    const size_t expected_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_RDCVA, expected_payload);

    size_t byte_count = ltc6811_1_rdcv_encode_broadcast(&ltc6811_1, LTC6811_1_CVXR_COUNT, payload);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, byte_count, "Encoded byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_payload, payload, BUFFER_LENGTH, "Encoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdcv_decode_with_valid_payload(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
#define VOLTAGE_COUNT (LTC6811_1_REG_CELL_COUNT * LTC6811_1_COUNT)
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    uint16_t voltages[VOLTAGE_COUNT] = { 0 };
    uint16_t expected_voltages[VOLTAGE_COUNT] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        const size_t index = ltc * LTC6811_1_REG_CELL_COUNT;
        expected_voltages[index + 0] = 0x0ff0;
        expected_voltages[index + 1] = 0xf00f;
        expected_voltages[index + 2] = 0x0ff0;

        payload[expected_byte_count + 0] = 0xf0;
        payload[expected_byte_count + 1] = 0x0f;
        payload[expected_byte_count + 2] = 0x0f;
        payload[expected_byte_count + 3] = 0xf0;
        payload[expected_byte_count + 4] = 0xf0;
        payload[expected_byte_count + 5] = 0x0f;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    const size_t decoded_byte_count = ltc6811_1_rdcv_decode_broadcast(&ltc6811_1, payload, voltages);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_voltages, voltages, sizeof(voltages[0]), LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
#undef VOLTAGE_COUNT
}

void test_rdcv_decode_decoded_payload_same_order_with_valid_voltages(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
#define VOLTAGE_COUNT (LTC6811_1_REG_CELL_COUNT * LTC6811_1_COUNT)
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    uint16_t voltages[VOLTAGE_COUNT] = { 0 };
    uint16_t expected_voltages[VOLTAGE_COUNT] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        const size_t index = ltc * LTC6811_1_REG_CELL_COUNT;
        expected_voltages[index] = ltc;

        payload[expected_byte_count] = ltc;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    const size_t decoded_byte_count = ltc6811_1_rdcv_decode_broadcast(&ltc6811_1, payload, voltages);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_voltages, voltages, sizeof(voltages[0]), VOLTAGE_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
#undef VOLTAGE_COUNT
}

void test_rdcv_decode_with_invalid_pec(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
#define VOLTAGE_COUNT (LTC6811_1_REG_CELL_COUNT * LTC6811_1_COUNT)
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    uint16_t voltages[VOLTAGE_COUNT] = { 0 };
    uint16_t expected_voltages[VOLTAGE_COUNT] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        if (ltc > 0) {
            const size_t index = ltc * LTC6811_1_REG_CELL_COUNT;
            expected_voltages[index + 0] = 0x0ff0;
            expected_voltages[index + 1] = 0xf00f;
            expected_voltages[index + 2] = 0x0ff0;
        }

        payload[expected_byte_count + 0] = 0xf0;
        payload[expected_byte_count + 1] = 0x0f;
        payload[expected_byte_count + 2] = 0x0f;
        payload[expected_byte_count + 3] = 0xf0;
        payload[expected_byte_count + 4] = 0xf0;
        payload[expected_byte_count + 5] = 0x0f;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }
    // Set PEC of the first IC to an invalid value
    const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    payload[6] = payload[7] = 0;

    const size_t decoded_byte_count = ltc6811_1_rdcv_decode_broadcast(&ltc6811_1, payload, voltages);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count - payload_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(&expected_voltages[0], &voltages[0], sizeof(voltages[0]), LTC6811_1_REG_CELL_COUNT, "Payload with wrong PEC was decoded");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(&expected_voltages[1], &voltages[1], sizeof(voltages[1]), VOLTAGE_COUNT - LTC6811_1_REG_CELL_COUNT, "Payload with correct PEC was not decoded");
#undef BUFFER_LENGTH
#undef VOLTAGE_COUNT
}

void test_rdcv_decode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
#define VOLTAGE_COUNT (LTC6811_1_REG_CELL_COUNT * LTC6811_1_COUNT)
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    uint16_t voltages[VOLTAGE_COUNT] = { 0 };
    uint16_t expected_voltages[VOLTAGE_COUNT] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdcv_decode_broadcast(NULL, payload, voltages);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_voltages, voltages, sizeof(voltages[0]), VOLTAGE_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
#undef VOLTAGE_COUNT
}

void test_rdcv_decode_with_null_payload(void) {
#define VOLTAGE_COUNT (LTC6811_1_REG_CELL_COUNT * LTC6811_1_COUNT)
    uint16_t voltages[VOLTAGE_COUNT] = { 0 };
    uint16_t expected_voltages[VOLTAGE_COUNT] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdcv_decode_broadcast(&ltc6811_1, NULL, voltages);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_voltages, voltages, sizeof(voltages[0]), VOLTAGE_COUNT, "Decoded payload data do not match");
#undef VOLTAGE_COUNT
}

void test_rdcv_decode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdcv_decode_broadcast(&ltc6811_1, payload, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            rdaux Test the read auxiliary voltage command
 * @{
 */

void test_rdaux_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_READ_BUFFER_SIZE)
    uint8_t expected_payload[BUFFER_LENGTH] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t byte_count = ltc6811_1_rdaux_encode_broadcast(NULL, LTC6811_1_AVAR, payload);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0, byte_count, "Encoded byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_payload, payload, BUFFER_LENGTH, "Encoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdaux_encode_with_null_output_array(void) {
    size_t byte_count = ltc6811_1_rdaux_encode_broadcast(&ltc6811_1, LTC6811_1_AVAR, NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0, byte_count, "Decoded byte count do not match");
}

void test_rdaux_encode_with_invalid_register(void) {
#define BUFFER_LENGTH (LTC6811_1_READ_BUFFER_SIZE)
    uint8_t expected_payload[BUFFER_LENGTH] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    const size_t expected_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_RDAUXA, expected_payload);

    size_t byte_count = ltc6811_1_rdaux_encode_broadcast(&ltc6811_1, LTC6811_1_AVXR_COUNT, payload);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, byte_count, "Encoded byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_payload, payload, BUFFER_LENGTH, "Encoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdaux_decode_with_valid_payload(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
#define VOLTAGE_COUNT (LTC6811_1_REG_AUX_COUNT * LTC6811_1_COUNT)
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    uint16_t voltages[VOLTAGE_COUNT] = { 0 };
    uint16_t expected_voltages[VOLTAGE_COUNT] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        const size_t index = ltc * LTC6811_1_REG_AUX_COUNT;
        expected_voltages[index + 0] = 0x0ff0;
        expected_voltages[index + 1] = 0xf00f;
        expected_voltages[index + 2] = 0x0ff0;

        payload[expected_byte_count + 0] = 0xf0;
        payload[expected_byte_count + 1] = 0x0f;
        payload[expected_byte_count + 2] = 0x0f;
        payload[expected_byte_count + 3] = 0xf0;
        payload[expected_byte_count + 4] = 0xf0;
        payload[expected_byte_count + 5] = 0x0f;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    const size_t decoded_byte_count = ltc6811_1_rdaux_decode_broadcast(&ltc6811_1, payload, voltages);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_voltages, voltages, sizeof(voltages[0]), LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
#undef VOLTAGE_COUNT
}

void test_rdaux_decode_decoded_payload_same_order_with_valid_voltages(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
#define VOLTAGE_COUNT (LTC6811_1_REG_AUX_COUNT * LTC6811_1_COUNT)
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    uint16_t voltages[VOLTAGE_COUNT] = { 0 };
    uint16_t expected_voltages[VOLTAGE_COUNT] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        const size_t index = ltc * LTC6811_1_REG_AUX_COUNT;
        expected_voltages[index] = ltc;

        payload[expected_byte_count] = ltc;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    const size_t decoded_byte_count = ltc6811_1_rdaux_decode_broadcast(&ltc6811_1, payload, voltages);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_voltages, voltages, sizeof(voltages[0]), VOLTAGE_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
#undef VOLTAGE_COUNT
}

void test_rdaux_decode_with_invalid_pec(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
#define VOLTAGE_COUNT (LTC6811_1_REG_AUX_COUNT * LTC6811_1_COUNT)
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    uint16_t voltages[VOLTAGE_COUNT] = { 0 };
    uint16_t expected_voltages[VOLTAGE_COUNT] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        if (ltc > 0) {
            const size_t index = ltc * LTC6811_1_REG_AUX_COUNT;
            expected_voltages[index + 0] = 0x0ff0;
            expected_voltages[index + 1] = 0xf00f;
            expected_voltages[index + 2] = 0x0ff0;
        }

        payload[expected_byte_count + 0] = 0xf0;
        payload[expected_byte_count + 1] = 0x0f;
        payload[expected_byte_count + 2] = 0x0f;
        payload[expected_byte_count + 3] = 0xf0;
        payload[expected_byte_count + 4] = 0xf0;
        payload[expected_byte_count + 5] = 0x0f;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }
    // Set PEC of the first IC to an invalid value
    const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    payload[6] = payload[7] = 0;

    const size_t decoded_byte_count = ltc6811_1_rdaux_decode_broadcast(&ltc6811_1, payload, voltages);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count - payload_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(&expected_voltages[0], &voltages[0], sizeof(voltages[0]), LTC6811_1_REG_AUX_COUNT, "Payload with wrong PEC was decoded");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(&expected_voltages[1], &voltages[1], sizeof(voltages[1]), VOLTAGE_COUNT - LTC6811_1_REG_AUX_COUNT, "Payload with correct PEC was not decoded");
#undef BUFFER_LENGTH
#undef VOLTAGE_COUNT
}

void test_rdaux_decode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
#define VOLTAGE_COUNT (LTC6811_1_REG_AUX_COUNT * LTC6811_1_COUNT)
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    uint16_t voltages[VOLTAGE_COUNT] = { 0 };
    uint16_t expected_voltages[VOLTAGE_COUNT] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdaux_decode_broadcast(NULL, payload, voltages);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_voltages, voltages, sizeof(voltages[0]), VOLTAGE_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
#undef VOLTAGE_COUNT
}

void test_rdaux_decode_with_null_payload(void) {
#define VOLTAGE_COUNT (LTC6811_1_REG_AUX_COUNT * LTC6811_1_COUNT)
    uint16_t voltages[VOLTAGE_COUNT] = { 0 };
    uint16_t expected_voltages[VOLTAGE_COUNT] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdaux_decode_broadcast(&ltc6811_1, NULL, voltages);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_voltages, voltages, sizeof(voltages[0]), VOLTAGE_COUNT, "Decoded payload data do not match");
#undef VOLTAGE_COUNT
}

void test_rdaux_decode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdaux_decode_broadcast(&ltc6811_1, payload, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            rdstat Test the read status command
 * @{
 */

void test_rdstat_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_READ_BUFFER_SIZE)
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };
    size_t encoded_byte_count = ltc6811_1_rdstat_encode_broadcast(NULL, LTC6811_1_STAR, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_rdstat_encode_with_null_output_array(void) {
    size_t encoded_byte_count = ltc6811_1_rdstat_encode_broadcast(&ltc6811_1, LTC6811_1_STAR, NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

void test_rdstat_encode_with_invalid_register(void) {
#define BUFFER_LENGTH (LTC6811_1_READ_BUFFER_SIZE)
    uint8_t expected_payload[BUFFER_LENGTH] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    const size_t expected_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_RDSTATA, expected_payload);

    size_t byte_count = ltc6811_1_rdstat_encode_broadcast(&ltc6811_1, LTC6811_1_STXR_COUNT, payload);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, byte_count, "Encoded byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_payload, payload, BUFFER_LENGTH, "Encoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdstat_decode_with_valid_payload(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Str expected_status[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Str status[LTC6811_1_COUNT] = { 0 };
    uint8_t payload[LTC6811_1_STXR_COUNT][BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count[LTC6811_1_STXR_COUNT] = { 0U };
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        // Expected Status struct
        expected_status[ltc].SC = 0b0101010110101010;
        expected_status[ltc].ITMP = 0b0101010110101010;
        expected_status[ltc].VA = 0b0101010110101010;

        expected_status[ltc].VD = 0b0101010110101010;
        expected_status[ltc].CUV = 0b000011110000;
        expected_status[ltc].COV = 0b111100001111;
        expected_status[ltc].THSD = 0b1;
        expected_status[ltc].MUXFAIL = 0b0;
        expected_status[ltc].RSVD = 0b01;
        expected_status[ltc].REV = 0b0101;

        // Prepare payloads
        for (enum Ltc68111Stxr reg = 0; reg < LTC6811_1_STXR_COUNT; ++reg) {
            payload[reg][expected_byte_count[reg] + 0] = 0b10101010;
            payload[reg][expected_byte_count[reg] + 1] = 0b01010101;
            payload[reg][expected_byte_count[reg] + 2] = 0b10101010;
            payload[reg][expected_byte_count[reg] + 3] = 0b01010101;
            payload[reg][expected_byte_count[reg] + 4] = 0b10101010;
            payload[reg][expected_byte_count[reg] + 5] = 0b01010101;
            expected_byte_count[reg] += prv_ltc6811_1_pec_calc(
                payload[reg] + expected_byte_count[reg],
                LTC6811_1_REG_BYTE_COUNT);
        }
    }

    size_t decoded_byte_count[LTC6811_1_STXR_COUNT] = { 0 };
    for (enum Ltc68111Stxr reg = 0; reg < LTC6811_1_STXR_COUNT; ++reg) {
        decoded_byte_count[reg] = ltc6811_1_rdstat_decode_broadcast(&ltc6811_1, reg, payload[reg], status);
    }

    TEST_ASSERT_EQUAL_size_t_ARRAY_MESSAGE(expected_byte_count, decoded_byte_count, LTC6811_1_STXR_COUNT, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_status, status, sizeof(status[0]), LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdstat_decode_decoded_payload_same_order_with_valid_status(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Str expected_status[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Str status[LTC6811_1_COUNT] = { 0 };
    uint8_t payload[LTC6811_1_STXR_COUNT][BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count[LTC6811_1_STXR_COUNT] = { 0U };
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        // Expected Status struct
        expected_status[ltc].SC = ltc * LTC6811_1_STXR_COUNT + LTC6811_1_STAR;
        expected_status[ltc].VD = ltc * LTC6811_1_STXR_COUNT + LTC6811_1_STBR;

        // Prepare payloads
        for (enum Ltc68111Stxr reg = 0; reg < LTC6811_1_STXR_COUNT; ++reg) {
            payload[reg][expected_byte_count[reg] + 0] = ltc * LTC6811_1_STXR_COUNT + reg;
            expected_byte_count[reg] += prv_ltc6811_1_pec_calc(
                payload[reg] + expected_byte_count[reg],
                LTC6811_1_REG_BYTE_COUNT);
        }
    }

    size_t decoded_byte_count[LTC6811_1_STXR_COUNT] = { 0 };
    for (enum Ltc68111Stxr reg = 0; reg < LTC6811_1_STXR_COUNT; ++reg) {
        decoded_byte_count[reg] = ltc6811_1_rdstat_decode_broadcast(&ltc6811_1, reg, payload[reg], status);
    }

    TEST_ASSERT_EQUAL_size_t_ARRAY_MESSAGE(expected_byte_count, decoded_byte_count, LTC6811_1_STXR_COUNT, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_status, status, sizeof(status[0]), LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdstat_decode_with_invalid_pec(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(1U))
    struct Ltc68111Str expected_status = { 0 };
    struct Ltc68111Str status = { 0 };
    uint8_t payload[LTC6811_1_STXR_COUNT][BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count[LTC6811_1_STXR_COUNT] = { 0U };
    // Correctly decoded data in STBR register
    expected_status.SC = 0b0101010110101010;
    expected_status.ITMP = 0b0101010110101010;
    expected_status.VA = 0b0101010110101010;

    // Prepare payloads
    for (enum Ltc68111Stxr reg = 0; reg < LTC6811_1_STXR_COUNT; ++reg) {
        payload[reg][expected_byte_count[reg] + 0] = 0b10101010;
        payload[reg][expected_byte_count[reg] + 1] = 0b01010101;
        payload[reg][expected_byte_count[reg] + 2] = 0b10101010;
        payload[reg][expected_byte_count[reg] + 3] = 0b01010101;
        payload[reg][expected_byte_count[reg] + 4] = 0b10101010;
        payload[reg][expected_byte_count[reg] + 5] = 0b01010101;
        if (reg != LTC6811_1_STBR) {
            expected_byte_count[reg] += prv_ltc6811_1_pec_calc(
                payload[reg] + expected_byte_count[reg],
                LTC6811_1_REG_BYTE_COUNT);
        }
    }

    size_t decoded_byte_count[LTC6811_1_STXR_COUNT] = { 0 };
    for (enum Ltc68111Stxr reg = 0; reg < LTC6811_1_STXR_COUNT; ++reg) {
        decoded_byte_count[reg] = ltc6811_1_rdstat_decode_broadcast(&ltc6811_1, reg, payload[reg], &status);
    }

    TEST_ASSERT_EQUAL_size_t_ARRAY_MESSAGE(expected_byte_count, decoded_byte_count, LTC6811_1_STXR_COUNT, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(&expected_status, &status, sizeof(status), "Error while decoding payload data");
#undef BUFFER_LENGTH
}

void test_rdstat_decode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t payload[BUFFER_LENGTH] = { 0 };
    struct Ltc68111Str status[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Str expected_status[LTC6811_1_COUNT] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdstat_decode_broadcast(NULL, LTC6811_1_STAR, payload, status);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_status, status, sizeof(status[0]), LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdstat_decode_with_null_payload(void) {
    struct Ltc68111Str status[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Str expected_status[LTC6811_1_COUNT] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdstat_decode_broadcast(&ltc6811_1, LTC6811_1_STAR, NULL, status);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_status, status, sizeof(status[0]), LTC6811_1_COUNT, "Decoded payload data do not match");
}

void test_rdstat_decode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdstat_decode_broadcast(&ltc6811_1, LTC6811_1_STAR, payload, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            wrsctrl Test write S pin control command
 * @{
 */

void test_wrsctrl_encode_with_valid_sctrl(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t sctrl[LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };

    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        for (size_t i = 0; i < LTC6811_1_SCTRL_COUNT; i += 4) {
            const size_t index = ltc * LTC6811_1_SCTRL_COUNT + i;
            sctrl[index + 0] = 0b1010;
            sctrl[index + 1] = 0b1010;
            sctrl[index + 2] = 0b0101;
            sctrl[index + 3] = 0b0101;
        }

        const size_t cmd_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_WRSCTRL, expected);
        const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
        const size_t reverse_index = cmd_byte_count +
                                     (LTC6811_1_COUNT - ltc - 1U) * payload_byte_count;
        expected[reverse_index + 0] = 0b10101010;
        expected[reverse_index + 1] = 0b01010101;
        expected[reverse_index + 2] = 0b10101010;
        expected[reverse_index + 3] = 0b01010101;
        expected[reverse_index + 4] = 0b10101010;
        expected[reverse_index + 5] = 0b01010101;
        const size_t _ = prv_ltc6811_1_pec_calc(expected + reverse_index, LTC6811_1_REG_BYTE_COUNT);
        EAGLETRT_API_UNUSED(_);
    }

    const size_t encoded_byte_count = ltc6811_1_wrsctrl_encode_broadcast(&ltc6811_1, sctrl, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(BUFFER_LENGTH, encoded_byte_count, "Encoded bytes count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrsctrl_encode_encoded_payload_reverse_order_with_valid_sctrl(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t sctrl[LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };

    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        const size_t index = ltc * LTC6811_1_SCTRL_COUNT;
        sctrl[index] = ltc;

        const size_t cmd_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_WRSCTRL, expected);
        const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
        const size_t reverse_index = cmd_byte_count +
                                     (LTC6811_1_COUNT - ltc - 1U) * payload_byte_count;
        expected[reverse_index] = ltc;
        const size_t _ = prv_ltc6811_1_pec_calc(expected + reverse_index, LTC6811_1_REG_BYTE_COUNT);
        EAGLETRT_API_UNUSED(_);
    }

    const size_t encoded_byte_count = ltc6811_1_wrsctrl_encode_broadcast(&ltc6811_1, sctrl, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(BUFFER_LENGTH, encoded_byte_count, "Encoded bytes count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrsctrl_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t sctrl[LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrsctrl_encode_broadcast(NULL, sctrl, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrsctrl_encode_with_null_payload(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrsctrl_encode_broadcast(&ltc6811_1, NULL, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrsctrl_encode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t sctrl[LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrsctrl_encode_broadcast(&ltc6811_1, sctrl, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            rdsctrl Test the read S pin control command
 * @{
 */

void test_rdsctrl_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_READ_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    size_t encoded_byte_count = ltc6811_1_rdsctrl_encode_broadcast(NULL, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_rdsctrl_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_rdsctrl_encode_broadcast(&ltc6811_1, NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

void test_rdsctrl_decode_with_valid_payload(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t expected_sctrl[LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t sctrl[LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count = 0;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        for (size_t byte = 0; byte < LTC6811_1_SCTRL_COUNT; byte += 4) {
            const size_t index = ltc * LTC6811_1_SCTRL_COUNT + byte;
            expected_sctrl[index + 0] = 0b1010;
            expected_sctrl[index + 1] = 0b1010;
            expected_sctrl[index + 2] = 0b0101;
            expected_sctrl[index + 3] = 0b0101;
        }

        payload[expected_byte_count + 0] = 0b10101010;
        payload[expected_byte_count + 1] = 0b01010101;
        payload[expected_byte_count + 2] = 0b10101010;
        payload[expected_byte_count + 3] = 0b01010101;
        payload[expected_byte_count + 4] = 0b10101010;
        payload[expected_byte_count + 5] = 0b01010101;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    const size_t decoded_byte_count = ltc6811_1_rdsctrl_decode_broadcast(&ltc6811_1, payload, sctrl);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_sctrl, sctrl, LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdsctrl_decode_decoded_payload_same_order_with_valid_sctrl(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t expected_sctrl[LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t sctrl[LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        expected_sctrl[ltc * LTC6811_1_SCTRL_COUNT] = ltc;

        // Prepare payloads
        payload[expected_byte_count] = ltc;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    size_t decoded_byte_count = ltc6811_1_rdsctrl_decode_broadcast(&ltc6811_1, payload, sctrl);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_sctrl, sctrl, LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdsctrl_decode_with_invalid_pec(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(1U))
    uint8_t sctrl[LTC6811_1_SCTRL_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    // Prepare payloads
    payload[0] = 0b10101010;
    payload[1] = 0b01010101;
    payload[2] = 0b10101010;
    payload[3] = 0b01010101;
    payload[4] = 0b10101010;
    payload[5] = 0b01010101;

    const size_t decoded_byte_count = ltc6811_1_rdsctrl_decode_broadcast(&ltc6811_1, payload, sctrl);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, sctrl, LTC6811_1_SCTRL_COUNT, "Error while decoding payload data");
#undef BUFFER_LENGTH
}

void test_rdsctrl_decode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t sctrl[LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdsctrl_decode_broadcast(NULL, payload, sctrl);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, sctrl, LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdsctrl_decode_with_null_payload(void) {
    uint8_t sctrl[LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdsctrl_decode_broadcast(&ltc6811_1, NULL, sctrl);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, sctrl, LTC6811_1_SCTRL_COUNT * LTC6811_1_COUNT, "Decoded payload data do not match");
}

void test_rdsctrl_decode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdsctrl_decode_broadcast(&ltc6811_1, payload, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            stsctrl Test the Self Test S pin control command
 * @{
 */

void test_stsctrl_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    size_t encoded_byte_count = ltc6811_1_stsctrl_encode_broadcast(NULL, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_stsctrl_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_stsctrl_encode_broadcast(&ltc6811_1, NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            clrsctrl Test the Clear S pin control command
 * @{
 */

void test_clrsctrl_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    size_t encoded_byte_count = ltc6811_1_clrsctrl_encode_broadcast(NULL, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_clrsctrl_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_clrsctrl_encode_broadcast(&ltc6811_1, NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            wrpwm Test write PWM command
 * @{
 */

void test_wrpwm_encode_with_valid_pwm(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t pwm[LTC6811_1_PWM_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };

    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        for (size_t i = 0; i < LTC6811_1_PWM_COUNT; i += 4) {
            const size_t index = ltc * LTC6811_1_PWM_COUNT + i;
            pwm[index + 0] = 0b1010;
            pwm[index + 1] = 0b1010;
            pwm[index + 2] = 0b0101;
            pwm[index + 3] = 0b0101;
        }

        const size_t cmd_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_WRPWM, expected);
        const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
        const size_t reverse_index = cmd_byte_count +
                                     (LTC6811_1_COUNT - ltc - 1U) * payload_byte_count;
        expected[reverse_index + 0] = 0b10101010;
        expected[reverse_index + 1] = 0b01010101;
        expected[reverse_index + 2] = 0b10101010;
        expected[reverse_index + 3] = 0b01010101;
        expected[reverse_index + 4] = 0b10101010;
        expected[reverse_index + 5] = 0b01010101;
        const size_t _ = prv_ltc6811_1_pec_calc(expected + reverse_index, LTC6811_1_REG_BYTE_COUNT);
        EAGLETRT_API_UNUSED(_);
    }

    const size_t encoded_byte_count = ltc6811_1_wrpwm_encode_broadcast(&ltc6811_1, pwm, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(BUFFER_LENGTH, encoded_byte_count, "Encoded bytes count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrpwm_encode_encoded_payload_reverse_order_with_valid_pwm(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t pwm[LTC6811_1_PWM_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };

    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        const size_t index = ltc * LTC6811_1_PWM_COUNT;
        pwm[index] = ltc;

        const size_t cmd_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_WRPWM, expected);
        const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
        const size_t reverse_index = cmd_byte_count +
                                     (LTC6811_1_COUNT - ltc - 1U) * payload_byte_count;
        expected[reverse_index] = ltc;
        const size_t _ = prv_ltc6811_1_pec_calc(expected + reverse_index, LTC6811_1_REG_BYTE_COUNT);
        EAGLETRT_API_UNUSED(_);
    }

    const size_t encoded_byte_count = ltc6811_1_wrpwm_encode_broadcast(&ltc6811_1, pwm, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(BUFFER_LENGTH, encoded_byte_count, "Encoded bytes count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrpwm_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t pwm[LTC6811_1_PWM_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrpwm_encode_broadcast(NULL, pwm, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrpwm_encode_with_null_payload(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrpwm_encode_broadcast(&ltc6811_1, NULL, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrpwm_encode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t pwm[LTC6811_1_PWM_COUNT * LTC6811_1_COUNT] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrpwm_encode_broadcast(&ltc6811_1, pwm, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            rdpwm Test read PWM command
 * @{
 */

void test_rdpwm_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_READ_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    size_t encoded_byte_count = ltc6811_1_rdpwm_encode_broadcast(NULL, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_rdpwm_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_rdpwm_encode_broadcast(&ltc6811_1, NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

void test_rdpwm_decode_with_valid_payload(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t expected_pwm[LTC6811_1_PWM_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t pwm[LTC6811_1_PWM_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count = 0;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        for (size_t byte = 0; byte < LTC6811_1_PWM_COUNT; byte += 4) {
            const size_t index = ltc * LTC6811_1_PWM_COUNT + byte;
            expected_pwm[index + 0] = 0b1010;
            expected_pwm[index + 1] = 0b1010;
            expected_pwm[index + 2] = 0b0101;
            expected_pwm[index + 3] = 0b0101;
        }

        payload[expected_byte_count + 0] = 0b10101010;
        payload[expected_byte_count + 1] = 0b01010101;
        payload[expected_byte_count + 2] = 0b10101010;
        payload[expected_byte_count + 3] = 0b01010101;
        payload[expected_byte_count + 4] = 0b10101010;
        payload[expected_byte_count + 5] = 0b01010101;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    const size_t decoded_byte_count = ltc6811_1_rdpwm_decode_broadcast(&ltc6811_1, payload, pwm);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_pwm, pwm, LTC6811_1_PWM_COUNT * LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdpwm_decode_decoded_payload_same_order_with_valid_pwm(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t expected_pwm[LTC6811_1_PWM_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t pwm[LTC6811_1_PWM_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        expected_pwm[ltc * LTC6811_1_PWM_COUNT] = ltc;

        // Prepare payloads
        payload[expected_byte_count] = ltc;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    size_t decoded_byte_count = ltc6811_1_rdpwm_decode_broadcast(&ltc6811_1, payload, pwm);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected_pwm, pwm, LTC6811_1_PWM_COUNT * LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdpwm_decode_with_invalid_pec(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(1U))
    uint8_t pwm[LTC6811_1_PWM_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    // Prepare payloads
    payload[0] = 0b10101010;
    payload[1] = 0b01010101;
    payload[2] = 0b10101010;
    payload[3] = 0b01010101;
    payload[4] = 0b10101010;
    payload[5] = 0b01010101;

    const size_t decoded_byte_count = ltc6811_1_rdpwm_decode_broadcast(&ltc6811_1, payload, pwm);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, pwm, LTC6811_1_PWM_COUNT, "Error while decoding payload data");
#undef BUFFER_LENGTH
}

void test_rdpwm_decode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t pwm[LTC6811_1_PWM_COUNT * LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdpwm_decode_broadcast(NULL, payload, pwm);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, pwm, LTC6811_1_PWM_COUNT * LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdpwm_decode_with_null_payload(void) {
    uint8_t pwm[LTC6811_1_PWM_COUNT * LTC6811_1_COUNT] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdpwm_decode_broadcast(&ltc6811_1, NULL, pwm);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, pwm, LTC6811_1_PWM_COUNT * LTC6811_1_COUNT, "Decoded payload data do not match");
}

void test_rdpwm_decode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdpwm_decode_broadcast(&ltc6811_1, payload, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            adcv Test ADC voltage measurement command
 * @{
 */

void test_adcv_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_adcv_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_DCP_DISABLED,
        LTC6811_1_CH_1,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_adcv_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_adcv_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_DCP_DISABLED,
        LTC6811_1_CH_1,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            adow Test ADC Open Wire check command
 * @{
 */

void test_adow_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_adow_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_PUP_INACTIVE,
        LTC6811_1_DCP_DISABLED,
        LTC6811_1_CH_1,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_adow_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_adow_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_PUP_INACTIVE,
        LTC6811_1_DCP_DISABLED,
        LTC6811_1_CH_1,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            cvst Test ADC Self Test command
 * @{
 */

void test_cvst_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_cvst_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_ST_ONE,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_cvst_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_cvst_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_ST_ONE,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            adol Test ADC overlap measurement command
 * @{
 */

void test_adol_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_adol_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_DCP_DISABLED,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_adol_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_adol_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_DCP_DISABLED,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            adax Test ADC auxiliary voltage measurement command
 * @{
 */

void test_adax_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_adax_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_CHG_GPIO_ALL,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_adax_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_adax_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_CHG_GPIO_ALL,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            adaxd Test ADC auxiliary voltage measurement with digital redundancy command
 * @{
 */

void test_adaxd_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_adaxd_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_CHG_GPIO_ALL,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_adaxd_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_adaxd_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_CHG_GPIO_ALL,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            axst Test ADC auxiliary voltage Self Test command
 * @{
 */

void test_axst_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_axst_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_ST_ONE,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_axst_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_axst_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_ST_ONE,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            adstat Test status group measurement command
 * @{
 */

void test_adstat_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_adstat_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_CHST_ALL,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_adstat_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_adstat_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_CHST_ALL,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            adstatd Test status group measurement with digital redundancy command
 * @{
 */

void test_adstatd_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_adstatd_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_CHST_ALL,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_adstatd_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_adstatd_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_CHST_ALL,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            statst Test status group Self Test command
 * @{
 */

void test_statst_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_statst_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_ST_ONE,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_statst_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_statst_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_ST_ONE,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            adcvax Test ADC combined cells and GPIOs voltage measurement command
 * @{
 */

void test_adcvax_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_adcvax_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_DCP_DISABLED,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_adcvax_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_adcvax_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_DCP_DISABLED,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            adcvsc Test ADC combined cells voltages and SC conversion measurement command
 * @{
 */

void test_adcvsc_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_adcvsc_encode_broadcast(
        NULL,
        LTC6811_1_MD_26HZ,
        LTC6811_1_DCP_DISABLED,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_adcvsc_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_adcvsc_encode_broadcast(
        &ltc6811_1,
        LTC6811_1_MD_26HZ,
        LTC6811_1_DCP_DISABLED,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            clrcell Test clear cell voltages command
 * @{
 */

void test_clrcell_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_clrcell_encode_broadcast(
        NULL,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_clrcell_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_clrcell_encode_broadcast(
        &ltc6811_1,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            clraux Test clear auxiliary voltages command
 * @{
 */

void test_clraux_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_clraux_encode_broadcast(
        NULL,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_clraux_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_clraux_encode_broadcast(
        &ltc6811_1,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            clrstat Test clear status groups command
 * @{
 */

void test_clrstat_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_clrstat_encode_broadcast(
        NULL,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_clrstat_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_clrstat_encode_broadcast(
        &ltc6811_1,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            pladc Test ADC poll status command
 * @{
 */

void test_pladc_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_pladc_encode_broadcast(
        NULL,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_pladc_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_pladc_encode_broadcast(
        &ltc6811_1,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            diagn Test ADC poll status command
 * @{
 */

void test_diagn_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_POLL_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_diagn_encode_broadcast(
        NULL,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_diagn_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_diagn_encode_broadcast(
        &ltc6811_1,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

/*!
 * \defgroup            wrcomm Test the write COMM command
 * @{
 */

void test_wrcomm_encode_with_valid_comm(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Comm comm[LTC6811_1_COUNT] = { 0 };
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };

    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        comm[ltc].icom0 = 0xa;
        comm[ltc].icom1 = 0xb;
        comm[ltc].icom2 = 0xc;
        comm[ltc].fcom0 = 0xd;
        comm[ltc].fcom1 = 0xe;
        comm[ltc].fcom2 = 0xf;
        for (size_t i = 0; i < LTC6811_1_COMM_COUNT; ++i) {
            comm[ltc].payload[i] = i + 1;
        }

        const size_t cmd_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_WRCOMM, expected);
        const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
        const size_t reverse_index = cmd_byte_count +
                                     (LTC6811_1_COUNT - ltc - 1U) * payload_byte_count;
        expected[reverse_index + 0] = 0xa0;
        expected[reverse_index + 1] = 0x1d;
        expected[reverse_index + 2] = 0xb0;
        expected[reverse_index + 3] = 0x2e;
        expected[reverse_index + 4] = 0xc0;
        expected[reverse_index + 5] = 0x3f;
        const size_t _ = prv_ltc6811_1_pec_calc(expected + reverse_index, LTC6811_1_REG_BYTE_COUNT);
        EAGLETRT_API_UNUSED(_);
    }

    const size_t encoded_byte_count = ltc6811_1_wrcomm_encode_broadcast(&ltc6811_1, comm, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(BUFFER_LENGTH, encoded_byte_count, "Encoded bytes count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrcomm_encode_encoded_payload_reverse_order_with_valid_comm(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Comm comm[LTC6811_1_COUNT] = { 0 };
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };

    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        comm[ltc].icom0 = ltc;

        const size_t cmd_byte_count = prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_WRCOMM, expected);
        const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
        const size_t reverse_index = cmd_byte_count +
                                     (LTC6811_1_COUNT - ltc - 1U) * payload_byte_count;
        expected[reverse_index] = ltc << 4;
        const size_t _ = prv_ltc6811_1_pec_calc(expected + reverse_index, LTC6811_1_REG_BYTE_COUNT);
        EAGLETRT_API_UNUSED(_);
    }

    const size_t encoded_byte_count = ltc6811_1_wrcomm_encode_broadcast(&ltc6811_1, comm, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(BUFFER_LENGTH, encoded_byte_count, "Encoded bytes count do not match");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrcomm_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Comm comm[LTC6811_1_COUNT] = { 0 };
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrcomm_encode_broadcast(NULL, comm, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrcomm_encode_with_null_comm(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrcomm_encode_broadcast(&ltc6811_1, NULL, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_wrcomm_encode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_WRITE_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Comm comm[LTC6811_1_COUNT] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_wrcomm_encode_broadcast(&ltc6811_1, comm, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            rdcomm Test the read COMM command
 * @{
 */

void test_rdcomm_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_READ_BUFFER_SIZE)
    uint8_t expected[BUFFER_LENGTH] = { 0 };
    uint8_t result[BUFFER_LENGTH] = { 0 };
    size_t encoded_byte_count = ltc6811_1_rdcomm_encode_broadcast(NULL, result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_rdcomm_encode_with_null_output_array(void) {
    size_t encoded_byte_count = ltc6811_1_rdcomm_encode_broadcast(&ltc6811_1, NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

void test_rdcomm_decode_with_valid_payload(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Comm expected_comm[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Comm comm[LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        expected_comm[ltc].icom0 = 0xa;
        expected_comm[ltc].icom1 = 0xb;
        expected_comm[ltc].icom2 = 0xc;
        expected_comm[ltc].fcom0 = 0xd;
        expected_comm[ltc].fcom1 = 0xe;
        expected_comm[ltc].fcom2 = 0xf;
        for (size_t i = 0; i < LTC6811_1_COMM_COUNT; ++i) {
            expected_comm[ltc].payload[i] = i + 1;
        }

        payload[expected_byte_count + 0] = 0xa0;
        payload[expected_byte_count + 1] = 0x1d;
        payload[expected_byte_count + 2] = 0xb0;
        payload[expected_byte_count + 3] = 0x2e;
        payload[expected_byte_count + 4] = 0xc0;
        payload[expected_byte_count + 5] = 0x3f;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    const size_t decoded_byte_count = ltc6811_1_rdcomm_decode_broadcast(&ltc6811_1, payload, comm);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_comm, comm, sizeof(comm[0]), LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdcomm_decode_decoded_payload_same_order_with_valid_comm(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Comm expected_comm[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Comm comm[LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        expected_comm[ltc].icom0 = ltc;
        payload[expected_byte_count] = ltc << 4U;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }

    const size_t decoded_byte_count = ltc6811_1_rdcomm_decode_broadcast(&ltc6811_1, payload, comm);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_comm, comm, sizeof(comm[0]), LTC6811_1_COUNT, "Decoded payload data do not match");
#undef BUFFER_LENGTH
}

void test_rdcomm_decode_with_invalid_pec(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Comm expected_comm[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Comm comm[LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    size_t expected_byte_count = 0U;
    for (size_t ltc = 0; ltc < LTC6811_1_COUNT; ++ltc) {
        if (ltc > 0) {
            expected_comm[ltc].icom0 = 0xa;
            expected_comm[ltc].icom1 = 0xb;
            expected_comm[ltc].icom2 = 0xc;
            expected_comm[ltc].fcom0 = 0xd;
            expected_comm[ltc].fcom1 = 0xe;
            expected_comm[ltc].fcom2 = 0xf;
            for (size_t i = 0; i < LTC6811_1_COMM_COUNT; ++i) {
                expected_comm[ltc].payload[i] = i + 1;
            }
        }

        payload[expected_byte_count + 0] = 0xa0;
        payload[expected_byte_count + 1] = 0x1d;
        payload[expected_byte_count + 2] = 0xb0;
        payload[expected_byte_count + 3] = 0x2e;
        payload[expected_byte_count + 4] = 0xc0;
        payload[expected_byte_count + 5] = 0x3f;
        expected_byte_count += prv_ltc6811_1_pec_calc(payload + expected_byte_count, LTC6811_1_REG_BYTE_COUNT);
    }
    // Set PEC of the first IC to an invalid value
    const size_t payload_byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    payload[6] = payload[7] = 0;

    const size_t decoded_byte_count = ltc6811_1_rdcomm_decode_broadcast(&ltc6811_1, payload, comm);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_byte_count - payload_byte_count, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(&expected_comm[0], &comm[0], sizeof(comm[0]), "Payload with wrong PEC was decoded");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(&expected_comm[1], &comm[1], sizeof(comm[1]), LTC6811_1_COUNT - 1, "Payload with correct PEC was not decoded");
#undef BUFFER_LENGTH
}

void test_rdcomm_decode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    struct Ltc68111Comm expected_comm[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Comm comm[LTC6811_1_COUNT] = { 0 };
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdcomm_decode_broadcast(NULL, payload, comm);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_comm, comm, sizeof(comm[0]), LTC6811_1_COUNT, "Decoded commuration has changed");
#undef BUFFER_LENGTH
}

void test_rdcomm_decode_with_null_payload(void) {
    struct Ltc68111Comm expected_comm[LTC6811_1_COUNT] = { 0 };
    struct Ltc68111Comm comm[LTC6811_1_COUNT] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdcomm_decode_broadcast(&ltc6811_1, NULL, comm);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
    TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected_comm, comm, sizeof(comm[0]), LTC6811_1_COUNT, "Decoded commuration has changed");
}

void test_rdcomm_decode_with_null_output_array(void) {
#define BUFFER_LENGTH (LTC6811_1_DATA_BUFFER_SIZE(LTC6811_1_COUNT))
    uint8_t payload[BUFFER_LENGTH] = { 0 };

    const size_t decoded_byte_count = ltc6811_1_rdcomm_decode_broadcast(&ltc6811_1, payload, NULL);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, decoded_byte_count, "Decoded byte count do not match");
#undef BUFFER_LENGTH
}

/*! @} */

/*!
 * \defgroup            stcomm Test COMM Start Transmission command
 * @{
 */

void test_stcomm_encode_contains_dummy_data(void) {
#define BUFFER_LENGTH (LTC6811_1_STCOMM_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_stcomm_encode_broadcast(
        &ltc6811_1,
        result);

    const size_t cmd_byte_count = LTC6811_1_CMD_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;

    TEST_ASSERT_EQUAL_size_t_MESSAGE(BUFFER_LENGTH, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0xff, result + cmd_byte_count, BUFFER_LENGTH - cmd_byte_count, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_stcomm_encode_with_null_handler(void) {
#define BUFFER_LENGTH (LTC6811_1_STCOMM_BUFFER_SIZE)
    uint8_t result[BUFFER_LENGTH] = { 0 };
    const size_t encoded_byte_count = ltc6811_1_stcomm_encode_broadcast(
        NULL,
        result);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
    TEST_ASSERT_EACH_EQUAL_UINT8_MESSAGE(0, result, BUFFER_LENGTH, "Encoded data do not match");
#undef BUFFER_LENGTH
}

void test_stcomm_encode_with_null_output_array(void) {
    const size_t encoded_byte_count = ltc6811_1_stcomm_encode_broadcast(
        &ltc6811_1,
        NULL);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(0U, encoded_byte_count, "Encoded bytes count is greater than 0");
}

/*! @} */

int main() {
    UNITY_BEGIN();

    /*!
     * \addgroup            enum Run enum definitions tests
     * @{
     */
    RUN_TEST(test_md_enum_fields_equivalency);
    RUN_TEST(test_ch_enum_fields_equivalency);
    /*! @} */

    /*!
     * \addgroup            struct Run struct definitions tests
     * @{
     */
    RUN_TEST(test_cfgr_struct_size_in_bytes);
    RUN_TEST(test_str_struct_size_in_bytes);
    RUN_TEST(test_comm_struct_size_in_bytes);
    /*! @} */

    /*!
     * \addgroup            pec Run Packet Error Code calculation tests
     * @{
     */
    RUN_TEST(test_pec_calculation_with_valid_payload);
    RUN_TEST(test_pec_calculation_with_null_payload);
    RUN_TEST(test_pec_calculation_with_zero_length);
    RUN_TEST(test_pec_calculation_and_join_with_valid_payload);
    RUN_TEST(test_pec_calculation_and_join_with_null_payload);
    RUN_TEST(test_pec_calculation_and_join_with_zero_length);

    RUN_TEST(test_pec_is_correct_with_matching_pec);
    RUN_TEST(test_pec_is_correct_with_non_matching_pec);
    RUN_TEST(test_pec_is_correct_with_null_payload);
    RUN_TEST(test_pec_is_correct_with_length_less_than_two);
    /*! @} */

    /*!
     * \addgroup            command Run command creation tests
     * @{
     */
    RUN_TEST(test_cmd_encode_with_valid_command);

    RUN_TEST(test_cmd_set_md_with_invalid_mode);
    RUN_TEST(test_cmd_set_pup_with_invalid_pullup_pulldown);
    RUN_TEST(test_cmd_set_st_with_invalid_mode);
    RUN_TEST(test_cmd_set_dcp_with_invalid_discharge_option);
    RUN_TEST(test_cmd_set_ch_with_invalid_cell);
    RUN_TEST(test_cmd_set_chg_with_invalid_gpio);
    RUN_TEST(test_cmd_set_chst_with_invalid_status);
    /*! @} */

    /*!
     * \addgroup            init Run initialization tests
     * @{
     */
    RUN_TEST(test_init_with_valid_parameters);
    RUN_TEST(test_init_with_null_handler);
    RUN_TEST(test_init_with_zero_count);
    /*! @} */

    /*!
     * \addgroup            wrcfg Run write configuration command tests
     * @{
     */
    RUN_TEST(test_wrcfg_encode_with_valid_configuration);
    RUN_TEST(test_wrcfg_encode_encoded_payload_reverse_order_with_valid_configuration);
    RUN_TEST(test_wrcfg_encode_with_null_handler);
    RUN_TEST(test_wrcfg_encode_with_null_configuration);
    RUN_TEST(test_wrcfg_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            rdcfg Run read configuration command tests
     * @{
     */
    RUN_TEST(test_rdcfg_encode_with_null_handler);
    RUN_TEST(test_rdcfg_encode_with_null_output_array);

    RUN_TEST(test_rdcfg_decode_with_valid_payload);
    RUN_TEST(test_rdcfg_decode_decoded_payload_same_order_with_valid_configuration);
    RUN_TEST(test_rdcfg_decode_with_invalid_pec);
    RUN_TEST(test_rdcfg_decode_with_null_handler);
    RUN_TEST(test_rdcfg_decode_with_null_payload);
    RUN_TEST(test_rdcfg_decode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            rdcv Run read voltage command tests
     * @{
     */
    RUN_TEST(test_rdcv_encode_with_null_handler);
    RUN_TEST(test_rdcv_encode_with_null_output_array);
    RUN_TEST(test_rdcv_encode_with_invalid_register);

    RUN_TEST(test_rdcv_decode_with_valid_payload);
    RUN_TEST(test_rdcv_decode_decoded_payload_same_order_with_valid_voltages);
    RUN_TEST(test_rdcv_decode_with_invalid_pec);
    RUN_TEST(test_rdcv_decode_with_null_handler);
    RUN_TEST(test_rdcv_decode_with_null_payload);
    RUN_TEST(test_rdcv_decode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            rdaux Run read auxiliary voltage command tests
     * @{
     */
    RUN_TEST(test_rdaux_encode_with_null_handler);
    RUN_TEST(test_rdaux_encode_with_null_output_array);
    RUN_TEST(test_rdaux_encode_with_invalid_register);

    RUN_TEST(test_rdaux_decode_with_valid_payload);
    RUN_TEST(test_rdaux_decode_decoded_payload_same_order_with_valid_voltages);
    RUN_TEST(test_rdaux_decode_with_invalid_pec);
    RUN_TEST(test_rdaux_decode_with_null_handler);
    RUN_TEST(test_rdaux_decode_with_null_payload);
    RUN_TEST(test_rdaux_decode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            rdstat Run read status command tests
     * @{
     */
    RUN_TEST(test_rdstat_encode_with_null_handler);
    RUN_TEST(test_rdstat_encode_with_null_output_array);
    RUN_TEST(test_rdstat_encode_with_invalid_register);

    RUN_TEST(test_rdstat_decode_with_valid_payload);
    RUN_TEST(test_rdstat_decode_decoded_payload_same_order_with_valid_status);
    RUN_TEST(test_rdstat_decode_with_invalid_pec);
    RUN_TEST(test_rdstat_decode_with_null_handler);
    RUN_TEST(test_rdstat_decode_with_null_payload);
    RUN_TEST(test_rdstat_decode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            wrsctrl Run write S pin control command tests
     * @{
     */
    RUN_TEST(test_wrsctrl_encode_with_valid_sctrl);
    RUN_TEST(test_wrsctrl_encode_encoded_payload_reverse_order_with_valid_sctrl);
    RUN_TEST(test_wrsctrl_encode_with_null_handler);
    RUN_TEST(test_wrsctrl_encode_with_null_payload);
    RUN_TEST(test_wrsctrl_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            rdsctrl Run read S pin control command tests
     * @{
     */
    RUN_TEST(test_rdsctrl_encode_with_null_handler);
    RUN_TEST(test_rdsctrl_encode_with_null_output_array);

    RUN_TEST(test_rdsctrl_decode_with_valid_payload);
    RUN_TEST(test_rdsctrl_decode_decoded_payload_same_order_with_valid_sctrl);
    RUN_TEST(test_rdsctrl_decode_with_invalid_pec);
    RUN_TEST(test_rdsctrl_decode_with_null_handler);
    RUN_TEST(test_rdsctrl_decode_with_null_payload);
    RUN_TEST(test_rdsctrl_decode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            stsctrl Run Self Test S pin control command tests
     * @{
     */
    RUN_TEST(test_stsctrl_encode_with_null_handler);
    RUN_TEST(test_stsctrl_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            clrsctrl Run Clear S pin control command tests
     * @{
     */
    RUN_TEST(test_clrsctrl_encode_with_null_handler);
    RUN_TEST(test_clrsctrl_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            wrpwm Run write PWM command tests
     * @{
     */
    RUN_TEST(test_wrpwm_encode_with_valid_pwm);
    RUN_TEST(test_wrpwm_encode_encoded_payload_reverse_order_with_valid_pwm);
    RUN_TEST(test_wrpwm_encode_with_null_handler);
    RUN_TEST(test_wrpwm_encode_with_null_payload);
    RUN_TEST(test_wrpwm_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            rdpwm Run read PWM command tests
     * @{
     */
    RUN_TEST(test_rdpwm_encode_with_null_handler);
    RUN_TEST(test_rdpwm_encode_with_null_output_array);

    RUN_TEST(test_rdpwm_decode_with_valid_payload);
    RUN_TEST(test_rdpwm_decode_decoded_payload_same_order_with_valid_pwm);
    RUN_TEST(test_rdpwm_decode_with_invalid_pec);
    RUN_TEST(test_rdpwm_decode_with_null_handler);
    RUN_TEST(test_rdpwm_decode_with_null_payload);
    RUN_TEST(test_rdpwm_decode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            adcv Run ADC voltage measurement command tests
     * @{
     */
    RUN_TEST(test_adcv_encode_with_null_handler);
    RUN_TEST(test_adcv_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            adow Run ADC Open Wire check command tests
     * @{
     */
    RUN_TEST(test_adow_encode_with_null_handler);
    RUN_TEST(test_adow_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            cvst Run ADC Self Test command tests
     * @{
     */
    RUN_TEST(test_cvst_encode_with_null_handler);
    RUN_TEST(test_cvst_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            adol Run ADC overlap measurement command tests
     * @{
     */
    RUN_TEST(test_adol_encode_with_null_handler);
    RUN_TEST(test_adol_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            adax Run ADC auxiliary voltage measurement command tests
     * @{
     */
    RUN_TEST(test_adax_encode_with_null_handler);
    RUN_TEST(test_adax_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            adaxd Run ADC auxiliary voltage measurement with digital redundancy command tests
     * @{
     */
    RUN_TEST(test_adaxd_encode_with_null_handler);
    RUN_TEST(test_adaxd_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            axst Run ADC auxiliary voltage Self Test command tests
     * @{
     */
    RUN_TEST(test_axst_encode_with_null_handler);
    RUN_TEST(test_axst_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            adstat Run status group measurement command tests
     * @{
     */
    RUN_TEST(test_adstat_encode_with_null_handler);
    RUN_TEST(test_adstat_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            adstatd Run status group measurement with digital redundancy command tests
     * @{
     */
    RUN_TEST(test_adstatd_encode_with_null_handler);
    RUN_TEST(test_adstatd_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            statst Run status group Self Test command tests
     * @{
     */
    RUN_TEST(test_statst_encode_with_null_handler);
    RUN_TEST(test_statst_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            adcvax Run ADC combined cells and GPIOs voltage measurement command tests
     * @{
     */
    RUN_TEST(test_adcvax_encode_with_null_handler);
    RUN_TEST(test_adcvax_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            adcvsc Run ADC combined cells voltages and SC conversion measurement command tests
     * @{
     */
    RUN_TEST(test_adcvsc_encode_with_null_handler);
    RUN_TEST(test_adcvsc_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            clrcell Run clear cell voltages command tests
     * @{
     */
    RUN_TEST(test_clrcell_encode_with_null_handler);
    RUN_TEST(test_clrcell_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            clraux Run clear auxiliary voltages command tests
     * @{
     */
    RUN_TEST(test_clraux_encode_with_null_handler);
    RUN_TEST(test_clraux_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            clrstat Run clear status groups command tests
     * @{
     */
    RUN_TEST(test_clrstat_encode_with_null_handler);
    RUN_TEST(test_clrstat_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            pladc Run ADC poll status command tests
     * @{
     */
    RUN_TEST(test_pladc_encode_with_null_handler);
    RUN_TEST(test_pladc_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            diagn Run ADC poll status command tests
     * @{
     */
    RUN_TEST(test_diagn_encode_with_null_handler);
    RUN_TEST(test_diagn_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            wrcomm Run write COMM command tests
     * @{
     */
    RUN_TEST(test_wrcomm_encode_with_valid_comm);
    RUN_TEST(test_wrcomm_encode_encoded_payload_reverse_order_with_valid_comm);
    RUN_TEST(test_wrcomm_encode_with_null_handler);
    RUN_TEST(test_wrcomm_encode_with_null_comm);
    RUN_TEST(test_wrcomm_encode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            rdcomm Run read COMM command tests
     * @{
     */
    RUN_TEST(test_rdcomm_encode_with_null_handler);
    RUN_TEST(test_rdcomm_encode_with_null_output_array);

    RUN_TEST(test_rdcomm_decode_with_valid_payload);
    RUN_TEST(test_rdcomm_decode_decoded_payload_same_order_with_valid_comm);
    RUN_TEST(test_rdcomm_decode_with_invalid_pec);
    RUN_TEST(test_rdcomm_decode_with_null_handler);
    RUN_TEST(test_rdcomm_decode_with_null_payload);
    RUN_TEST(test_rdcomm_decode_with_null_output_array);
    /*! @} */

    /*!
     * \addgroup            stcomm Run COMM Start Transmission command tests
     * @{
     */
    RUN_TEST(test_stcomm_encode_contains_dummy_data);
    RUN_TEST(test_stcomm_encode_with_null_handler);
    RUN_TEST(test_stcomm_encode_with_null_output_array);
    /*! @} */

    UNITY_END();
}
