/*!
 * \file            ltc6811-1.c
 * \date            2023-12-16
 * \authors         Antonio Gelain [antonio.gelain2@gmail.com]
 *                  Riccardo Segala [riccardo.segala@eagletrt.it]
 *
 * \brief           Function implementations needed for the LTC6811-1.
 *
 * \details         For additional information refer to the datasheet:
 * \link            https://www.analog.com/media/en/technical-documentation/data-sheets/ltc6811-1-6811-2.pdf
 *
 * \warning         For SPI communication with the LTC6811-1, mode 3 is
 *                  required (in CubeMX is CPOL="Low" and CPHA="2 edge") and
 *                  the MISO pin has to be set in Pull-Up mode.
 *                  Maximum SPI communication speed with the LTC is 1MHz.
 */

#include "ltc6811-1.h"
#include "ltc6811-1-api.h"
#include "eagletrt-api.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

/*!
 * \brief           Table of precomputed values used to calculate the PEC for messaging
 */
// clang-format off
EAGLETRT_STATIC const uint16_t crcTable[] = {
    0x0000, 0xc599, 0xceab, 0x0b32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd,
    0xf407, 0x319e, 0x3aac, 0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa,
    0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1, 0xbbf3, 0x7e6a,
    0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d,
    0x5b2e, 0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3,
    0xaf29, 0x6ab0, 0x6182, 0xa41b, 0x77e6, 0xb27f, 0xb94d, 0x7cd4,
    0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd, 0x2544,
    0x02be, 0xc727, 0xcc15, 0x098c, 0xda71, 0x1fe8, 0x14da, 0xd143,
    0xf3c5, 0x365c, 0x3d6e, 0xf8f7, 0x2b0a, 0xee93, 0xe5a1, 0x2038,
    0x07c2, 0xc25b, 0xc969, 0x0cf0, 0xdf0d, 0x1a94, 0x11a6, 0xd43f,
    0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
    0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8,
    0xa8eb, 0x6d72, 0x6640, 0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16,
    0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba, 0x4a88, 0x8f11,
    0x057c, 0xc0e5, 0xcbd7, 0x0e4e, 0xddb3, 0x182a, 0x1318, 0xd681,
    0xf17b, 0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286,
    0xa213, 0x678a, 0x6cb8, 0xa921, 0x7adc, 0xbf45, 0xb477, 0x71ee,
    0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070, 0x85e9,
    0x0f84, 0xca1d, 0xc12f, 0x04b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79,
    0xfb83, 0x3e1a, 0x3528, 0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e,
    0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59, 0x2ac0,
    0x0d3a, 0xc8a3, 0xc391, 0x0608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7,
    0x54aa, 0x9133, 0x9a01, 0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757,
    0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9, 0x7350,
    0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b,
    0xa5d1, 0x6048, 0x6b7a, 0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c,
    0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25, 0x2fbc,
    0x0846, 0xcddf, 0xc6ed, 0x0374, 0xd089, 0x1510, 0x1e22, 0xdbbb,
    0x0af8, 0xcf61, 0xc453, 0x01ca, 0xd237, 0x17ae, 0x1c9c, 0xd905,
    0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b, 0x2d02,
    0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492,
    0x5368, 0x96f1, 0x9dc3, 0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095
};
// clang-format on

/*!
 * \brief           Generate the Packet Error Code (PEC) from an arbitrary
 *                  payload
 *
 * \param[in]       payload: The payload bytes
 * \param[in]       len: The length of the payload in bytes
 * \return          The calculated PEC
 */
EAGLETRT_STATIC uint16_t prv_ltc6811_1_pec15(const uint8_t *const payload, const size_t len) {
    const uint16_t pec_seed = 16;
    uint16_t remainder = pec_seed;
    for (size_t i = 0; i < len; ++i) {
        // calculate PEC table address
        uint16_t address = ((remainder >> 7) ^ payload[i]) & 0xff;
        remainder = (remainder << 8) ^ crcTable[address];
    }
    // The CRC15 has a 0 in the LSB so the final value must be multiplied by 2
    return remainder * 2;
}

/*!
 * \brief           Calculate and add the PEC to the payload array
 *
 * \warning         The array should have enough space to contain both the
 *                  payload and the 2 bytes of PEC
 *
 * \param[in, out]  payload: The payload bytes
 * \param[in]       len: The length of the payload in bytes, PEC excluded
 * \return          The length of the payload in bytes, PEC included
 */
EAGLETRT_STATIC_INLINE size_t prv_ltc6811_1_pec_calc(uint8_t *payload, size_t len) {
    uint16_t pec = prv_ltc6811_1_pec15(payload, len);
    const uint8_t pec_offset_high = 8U;
    payload[len] = (uint8_t)(pec >> pec_offset_high);
    payload[len + 1] = (uint8_t)pec;
    return len + LTC6811_1_PEC_BYTE_COUNT;
}

/*!
 * \brief           Check if the received PEC is correct
 *
 * \param[in]       payload: The payload bytes
 * \param[in]       len: Length of the payload in bytes, PEC included
 * \return          True if the given and calculated PEC match, false otherwise
 */
EAGLETRT_STATIC_INLINE bool prv_ltc6811_1_pec_is_correct(uint8_t *payload, size_t len) {
    const uint8_t pec_offset_high = 8U;
    uint16_t pec = ((uint16_t)payload[len - 2] << pec_offset_high) | (uint16_t)payload[len - 1];
    return prv_ltc6811_1_pec15(payload, len - LTC6811_1_PEC_BYTE_COUNT) == pec;
}

/*!
 * \brief           Copy the command and its calculated PEC inside a given
 *                  array
 *
 * \warning         The given array has to be at least
 *                  (\c LTC6811_1_CMD_BYTE_COUNT + \c LTC6811_1_PEC_BYTE_COUNT)
 *                  long. Only the first.
 *                  (\c LTC6811_1_CMD_BYTE_COUNT + \c LTC6811_1_PEC_BYTE_COUNT)
 *                  bytes are modified
 *
 * \param[in]       cmd: The command to copy
 * \param[out]      out: The array where command and PEC are stored
 * \return          The total number of encoded bytes, PEC included
 */
EAGLETRT_STATIC_INLINE size_t prv_ltc6811_1_cmd_encode(enum Ltc68111Command cmd, uint8_t *out) {
    const uint8_t cmd_offset_high = 8U;
    const uint8_t cmd_mask_high = 0x07;
    out[0] = (uint8_t)((cmd >> cmd_offset_high) & cmd_mask_high);
    out[1] = (uint8_t)cmd;
    return prv_ltc6811_1_pec_calc(out, 2);
}

/*!
 * \brief           Set the ADC conversion mode option inside the given command
 *
 * \param[in]       cmd: The command without the option set
 * \param[in]       mode: The ADC conversion mode
 * \return          The given command with the ADC conversion mode set
 */
EAGLETRT_STATIC_INLINE enum Ltc68111Command prv_ltc6811_1_cmd_set_md(enum Ltc68111Command cmd, enum Ltc68111Md mode) {
    const uint8_t mode_offset = 7U;
    return mode < LTC6811_1_MD_COUNT ? (cmd | (mode << mode_offset)) : cmd;
}

/*!
 * \brief           Set the Pull-up/Pull-down current for open-wire conversion
 *                  option inside the given command
 *
 * \param[in]       cmd: The command without the option set
 * \param[in]       pup: The Pull-up/Pull-down option
 * \return          The given command with the Pull-up/Pull-down option set
 */
EAGLETRT_STATIC_INLINE enum Ltc68111Command prv_ltc6811_1_cmd_set_pup(enum Ltc68111Command cmd, enum Ltc68111Pup pup) {
    const uint8_t pup_offset = 6U;
    return pup < LTC6811_1_PUP_COUNT ? (cmd | (pup << pup_offset)) : cmd;
}

/*!
 * \brief           Set the self test mode option inside the given command
 *
 * \param[in]       cmd: The command without the option set
 * \param[in]       mode: The self test mode option
 * \return          The given command with the self test mode set
 */
EAGLETRT_STATIC_INLINE enum Ltc68111Command prv_ltc6811_1_cmd_set_st(enum Ltc68111Command cmd, enum Ltc68111St mode) {
    const uint8_t mode_offset = 5U;
    return mode < LTC6811_1_ST_COUNT ? (cmd | (mode << mode_offset)) : cmd;
}

/*!
 * \brief           Set the discharge permitted option inside the given command
 *
 * \param[in]       cmd: The command without the option set
 * \param[in]       dcp: The discharge permitted option
 * \return          The given command with the discharge option set
 */
EAGLETRT_STATIC_INLINE enum Ltc68111Command prv_ltc6811_1_cmd_set_dcp(enum Ltc68111Command cmd, enum Ltc68111Dcp dcp) {
    const uint8_t dcp_offset = 4U;
    return dcp < LTC6811_1_DCP_COUNT ? (cmd | (dcp << dcp_offset)) : cmd;
}

/*!
 * \brief           Set the cell selection for ADC conversion inside the given
 *                  command
 *
 * \param[in]       cmd: The command without the option set
 * \param[in]       cells: The selected cells
 * \return          The given command with the selected cells set
 */
EAGLETRT_STATIC_INLINE enum Ltc68111Command prv_ltc6811_1_cmd_set_ch(enum Ltc68111Command cmd, enum Ltc68111Ch cells) {
    return cells < LTC6811_1_CH_COUNT ? (cmd | (uint8_t)cells) : cmd;
}

/*!
 * \brief           Set the GPIOs selection for ADC conversion inside the given
 *                  command
 *
 * \param[in]       cmd: The command without the option set
 * \param[in]       gpios: The selected GPIOs
 * \return          The given command with the selected GPIOs set
 */
EAGLETRT_STATIC_INLINE enum Ltc68111Command prv_ltc6811_1_cmd_set_chg(enum Ltc68111Command cmd, enum Ltc68111Chg gpios) {
    return gpios < LTC6811_1_CHG_COUNT ? (cmd | (uint8_t)gpios) : cmd;
}

/*!
 * \brief           Set the status group selection for ADC conversion inside
 *                  the given command
 *
 * \param[in]       cmd: The command without the option set
 * \param[in]       groups: The selected status groups
 * \return          The given command with the selected status groups set
 */
EAGLETRT_STATIC_INLINE enum Ltc68111Command prv_ltc6811_1_cmd_set_chst(enum Ltc68111Command cmd, enum Ltc68111Chst groups) {
    return groups < LTC6811_1_CHST_COUNT ? (cmd | (uint8_t)groups) : cmd;
}

void ltc6811_1_init(struct Ltc68111Handler *handler, size_t ltc_count) {
    if (handler == NULL) {
        return;
    }
    memset(handler, 0, sizeof(*handler));
    handler->count = EAGLETRT_API_MAX(1U, ltc_count);
}

bool ltc6811_1_pladc_is_completed(const uint8_t byte) {
    return byte == LTC6811_1_PLADC_COMPLETE_BYTE_VALUE;
}

size_t ltc6811_1_wrcfg_encode_broadcast(
    const struct Ltc68111Handler *handler,
    struct Ltc68111Cfgr *config,
    uint8_t *out) {
    if (handler == NULL || config == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_WRCFGA;
    size_t encoded = prv_ltc6811_1_cmd_encode(cmd, out);

    for (size_t i = 0; i < handler->count; ++i) {
        const size_t configIndex = handler->count - i - 1;
        const struct Ltc68111Cfgr *cfg = &(config[configIndex]);

        out[encoded] = (cfg->GPIO << 3) | (cfg->REFON << 2) | (cfg->DTEN << 1) | (cfg->ADCOPT);
        out[encoded + 1] = cfg->VUV;
        out[encoded + 2] = (cfg->VOV << 4) | (cfg->VUV >> 8);
        out[encoded + 3] = cfg->VOV >> 4;
        out[encoded + 4] = cfg->DCC;
        out[encoded + 5] = (cfg->DCTO << 4) | (cfg->DCC >> 8);

        encoded += prv_ltc6811_1_pec_calc(out + encoded, LTC6811_1_REG_BYTE_COUNT);
    }
    return encoded;
}

size_t ltc6811_1_rdcfg_encode_broadcast(const struct Ltc68111Handler *handler, uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }
    return prv_ltc6811_1_cmd_encode(LTC6811_1_CMD_RDCFGA, out);
}

size_t ltc6811_1_rdcfg_decode_broadcast(
    const struct Ltc68111Handler *handler,
    const uint8_t *payload,
    struct Ltc68111Cfgr *out) {
    if (handler == NULL || payload == NULL || out == NULL) {
        return 0U;
    }

    size_t decoded = 0, off = 0;
    const size_t byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    for (size_t i = 0; i < handler->count; ++i) {
        if (prv_ltc6811_1_pec_is_correct(payload + off, byte_count)) {
            // Decode configuration payload
            out[i].ADCOPT = payload[off] & 0x01;
            out[i].DTEN = (payload[off] & 0x02) >> 1;
            out[i].REFON = (payload[off] & 0x04) >> 2;
            out[i].GPIO = (payload[off] & 0xF8) >> 3;
            out[i].VUV = payload[off + 1] | (((uint16_t)payload[off + 2] & 0x0F) << 8);
            out[i].VOV = ((payload[off + 2] & 0xF0) >> 4) | ((uint16_t)payload[off + 3] << 4);
            out[i].DCC = payload[off + 4] | (((uint16_t)payload[off + 5] & 0x0F) << 8);
            out[i].DCTO = (payload[off + 5] & 0xF0) >> 4;

            decoded += byte_count;
        }
        off += byte_count;
    }
    return decoded;
}

size_t ltc6811_1_rdcv_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Cvxr reg,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_RDCVA;
    switch (reg) {
        case LTC6811_1_CVAR:
            cmd = LTC6811_1_CMD_RDCVA;
            break;
        case LTC6811_1_CVBR:
            cmd = LTC6811_1_CMD_RDCVB;
            break;
        case LTC6811_1_CVCR:
            cmd = LTC6811_1_CMD_RDCVC;
            break;
        case LTC6811_1_CVDR:
            cmd = LTC6811_1_CMD_RDCVD;
            break;
        default:
            cmd = LTC6811_1_CMD_RDCVA;
            break;
    }
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_rdcv_decode_broadcast(
    const struct Ltc68111Handler *handler,
    const uint8_t *payload,
    uint16_t *out) {
    if (handler == NULL || payload == NULL || out == NULL) {
        return 0;
    }

    size_t decoded = 0, off = 0;
    const size_t byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    for (size_t i = 0; i < handler->count; ++i) {
        if (prv_ltc6811_1_pec_is_correct(payload + off, byte_count)) {
            // For each cell voltage in register
            for (size_t j = 0; j < LTC6811_1_REG_CELL_COUNT; ++j) {
                size_t cell_off = off + j * sizeof(uint16_t);
                out[i * LTC6811_1_REG_CELL_COUNT + j] = payload[cell_off] | ((uint16_t)payload[cell_off + 1] << 8);
            }
            decoded += byte_count;
        }
        off += byte_count;
    }
    return decoded;
}

size_t ltc6811_1_rdaux_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Avxr reg,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_RDAUXA;
    switch (reg) {
        case LTC6811_1_AVAR:
            cmd = LTC6811_1_CMD_RDAUXA;
            break;
        case LTC6811_1_AVBR:
            cmd = LTC6811_1_CMD_RDAUXB;
            break;
        default:
            cmd = LTC6811_1_CMD_RDAUXA;
            break;
    }
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_rdaux_decode_broadcast(
    const struct Ltc68111Handler *handler,
    const uint8_t *payload,
    uint16_t *out) {
    if (handler == NULL || payload == NULL || out == NULL) {
        return 0U;
    }

    size_t decoded = 0, off = 0;
    const size_t byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    for (size_t i = 0; i < handler->count; ++i) {
        if (prv_ltc6811_1_pec_is_correct(payload + off, byte_count)) {
            // For each auxilary voltage in register
            for (size_t j = 0; j < LTC6811_1_REG_AUX_COUNT; ++j) {
                size_t cell_off = off + j * sizeof(uint16_t);
                out[i * LTC6811_1_REG_AUX_COUNT + j] = payload[cell_off] | ((uint16_t)payload[cell_off + 1] << 8);
            }
            decoded += byte_count;
        }
        off += byte_count;
    }
    return decoded;
}

size_t ltc6811_1_rdstat_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Stxr reg,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_RDSTATA;
    switch (reg) {
        case LTC6811_1_STAR:
            cmd = LTC6811_1_CMD_RDSTATA;
            break;
        case LTC6811_1_STBR:
            cmd = LTC6811_1_CMD_RDSTATB;
            break;
        default:
            cmd = LTC6811_1_CMD_RDSTATA;
            break;
    }
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_rdstat_decode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Stxr reg,
    const uint8_t *payload,
    struct Ltc68111Str *out) {
    if (handler == NULL || payload == NULL || out == NULL) {
        return 0U;
    }

    size_t decoded = 0, off = 0;
    const size_t byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    for (size_t i = 0; i < handler->count; ++i) {
        // Check PEC validity
        if (prv_ltc6811_1_pec_is_correct(payload + off, byte_count)) {
            switch (reg) {
                case LTC6811_1_STAR:
                    out[i].SC = payload[off] | ((uint16_t)payload[off + 1] << 8);
                    out[i].ITMP = payload[off + 2] | ((uint16_t)payload[off + 3] << 8);
                    out[i].VA = payload[off + 4] | ((uint16_t)payload[off + 5] << 8);
                    break;
                case LTC6811_1_STBR:
                    out[i].VD = payload[off] | ((uint16_t)payload[off + 1] << 8);
                    // Get CUV and COV bits
                    for (size_t byte = 0; byte < 3; ++byte) {
                        for (size_t b = 0; b < 4; ++b) {
                            uint16_t cuv = (payload[off + byte + 2] & (1 << (b * 2))) >> b;
                            uint16_t cov = (payload[off + byte + 2] & (1 << (b * 2 + 1))) >> (b + 1);
                            out[i].CUV |= cuv << (byte * 4);
                            out[i].COV |= cov << (byte * 4);
                        }
                    }
                    out[i].REV = (payload[off + 5] & 0b11110000) >> 4;
                    out[i].RSVD = (payload[off + 5] & 0b00001100) >> 2;
                    out[i].MUXFAIL = (payload[off + 5] & 0b00000010) >> 1;
                    out[i].THSD = (payload[off + 5] & 0b00000001);
                    break;
                default:
                    return 0U;
            }
            decoded += byte_count;
        }
        off += byte_count;
    }
    return decoded;
}

size_t ltc6811_1_wrsctrl_encode_broadcast(
    const struct Ltc68111Handler *handler,
    const uint8_t *payload,
    uint8_t *out) {
    if (handler == NULL || payload == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_WRSCTRL;
    size_t encoded = prv_ltc6811_1_cmd_encode(cmd, out);

    // Encode data with corresponsing PEC
    for (size_t i = 0; i < handler->count; ++i) {
        for (size_t byte = 0; byte < LTC6811_1_REG_BYTE_COUNT; ++byte) {
            const size_t ltcIndex = handler->count - i - 1;
            const size_t index = ltcIndex * LTC6811_1_REG_SCTRL_COUNT + byte * 2;
            out[encoded + byte] = (payload[index] & 0x0F) | (payload[index + 1] & 0x0F) << 4;
        }
        encoded += prv_ltc6811_1_pec_calc(out + encoded, LTC6811_1_REG_BYTE_COUNT);
    }
    return encoded;
}

size_t ltc6811_1_rdsctrl_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }
    enum Ltc68111Command cmd = LTC6811_1_CMD_RDSCTRL;
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_rdsctrl_decode_broadcast(
    const struct Ltc68111Handler *handler,
    const uint8_t *payload,
    uint8_t *out) {
    if (handler == NULL || payload == NULL || out == NULL) {
        return 0;
    }

    size_t decoded = 0, off = 0;
    const size_t byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    for (size_t i = 0; i < handler->count; ++i) {
        // Check PEC validity
        if (prv_ltc6811_1_pec_is_correct(payload + off, byte_count)) {
            // For each payload byte
            for (size_t byte = 0; byte < LTC6811_1_REG_BYTE_COUNT; ++byte) {
                size_t index = i * LTC6811_1_REG_SCTRL_COUNT + byte * 2;
                out[index] = payload[off + byte] & 0x0F;
                out[index + 1] = payload[off + byte] >> 4;
            }
            decoded += byte_count;
        }
        off += byte_count;
    }
    return decoded;
}

size_t ltc6811_1_stsctrl_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }
    enum Ltc68111Command cmd = LTC6811_1_CMD_STSCTRL;
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_clrsctrl_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }
    enum Ltc68111Command cmd = LTC6811_1_CMD_CLRSCTRL;
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_wrpwm_encode_broadcast(
    const struct Ltc68111Handler *handler,
    const uint8_t *payload,
    uint8_t *out) {
    if (handler == NULL || payload == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_WRPWM;
    size_t encoded = prv_ltc6811_1_cmd_encode(cmd, out);

    // Encode data with corresponsing PEC
    for (size_t i = 0; i < handler->count; ++i) {
        for (size_t byte = 0; byte < LTC6811_1_REG_BYTE_COUNT; ++byte) {
            const size_t ltcIndex = handler->count - i - 1;
            const size_t index = ltcIndex * LTC6811_1_REG_PWM_COUNT + byte * 2;
            out[encoded + byte] = (payload[index] & 0x0F) | (payload[index + 1] & 0x0F) << 4;
        }
        encoded += prv_ltc6811_1_pec_calc(out + encoded, LTC6811_1_REG_BYTE_COUNT);
    }
    return encoded;
}

size_t ltc6811_1_rdpwm_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }
    enum Ltc68111Command cmd = LTC6811_1_CMD_RDPWM;
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_rdpwm_decode_broadcast(
    const struct Ltc68111Handler *handler,
    const uint8_t *payload,
    uint8_t *out) {
    if (handler == NULL || payload == NULL || out == NULL) {
        return 0;
    }

    size_t decoded = 0, off = 0;
    const size_t byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    for (size_t i = 0; i < handler->count; ++i) {
        if (prv_ltc6811_1_pec_is_correct(payload + off, byte_count)) {
            // For each payload byte
            for (size_t byte = 0; byte < LTC6811_1_REG_BYTE_COUNT; ++byte) {
                size_t index = i * LTC6811_1_PWM_COUNT + byte * 2;
                out[index] = payload[off + byte] & 0x0F;
                out[index + 1] = payload[off + byte] >> 4;
            }
            decoded += byte_count;
        }
        off += byte_count;
    }
    return decoded;
}

size_t ltc6811_1_adcv_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Dcp dcp,
    enum Ltc68111Ch cells,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_ADCV;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_dcp(cmd, dcp);
    cmd = prv_ltc6811_1_cmd_set_ch(cmd, cells);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_adow_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Pup pup,
    enum Ltc68111Dcp dcp,
    enum Ltc68111Ch cells,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_ADOW;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_pup(cmd, pup);
    cmd = prv_ltc6811_1_cmd_set_dcp(cmd, dcp);
    cmd = prv_ltc6811_1_cmd_set_ch(cmd, cells);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_cvst_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111St test_mode,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_CVST;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_st(cmd, test_mode);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_adol_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Dcp dcp,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_ADOL;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_dcp(cmd, dcp);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_adax_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Chg gpios,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_ADAX;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_chg(cmd, gpios);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_adaxd_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Chg gpios,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_ADAXD;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_chg(cmd, gpios);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_axst_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111St test_mode,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_AXST;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_st(cmd, test_mode);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_adstat_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Chst groups,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_ADSTAT;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_chst(cmd, groups);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_adstatd_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Chst groups,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_ADSTATD;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_chst(cmd, groups);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_statst_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111St test_mode,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_STATST;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_st(cmd, test_mode);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_adcvax_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Dcp dcp,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_ADCVAX;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_dcp(cmd, dcp);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_adcvsc_encode_broadcast(
    const struct Ltc68111Handler *handler,
    enum Ltc68111Md mode,
    enum Ltc68111Dcp dcp,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_ADCVSC;
    cmd = prv_ltc6811_1_cmd_set_md(cmd, mode);
    cmd = prv_ltc6811_1_cmd_set_dcp(cmd, dcp);
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_clrcell_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_CLRCELL;
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_clraux_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_CLRAUX;
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_clrstat_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_CLRSTAT;
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_pladc_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_PLADC;
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_diagn_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_DIAGN;
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_wrcomm_encode_broadcast(
    const struct Ltc68111Handler *handler,
    struct Ltc68111Comm *comms,
    uint8_t *out) {
    if (handler == NULL || comms == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_WRCOMM;

    size_t encoded = prv_ltc6811_1_cmd_encode(cmd, out);
    for (size_t i = 0; i < handler->count; ++i) {
        struct Ltc68111Comm *comm = &(comms[handler->count - i - 1]);

        out[encoded] = (comm->icom0 << 4) | (comm->payload[0] >> 4);
        out[encoded + 1] = ((comm->payload[0] & 0x0F) << 4) | comm->fcom0;
        out[encoded + 2] = (comm->icom1 << 4) | (comm->payload[1] >> 4);
        out[encoded + 3] = ((comm->payload[1] & 0x0F) << 4) | comm->fcom1;
        out[encoded + 4] = (comm->icom2 << 4) | (comm->payload[2] >> 4);
        out[encoded + 5] = ((comm->payload[2] & 0x0F) << 4) | comm->fcom2;

        encoded += prv_ltc6811_1_pec_calc(out + encoded, LTC6811_1_REG_BYTE_COUNT);
    }
    return encoded;
}

size_t ltc6811_1_rdcomm_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_RDCOMM;
    return prv_ltc6811_1_cmd_encode(cmd, out);
}

size_t ltc6811_1_rdcomm_decode_broadcast(
    const struct Ltc68111Handler *handler,
    const uint8_t *payload,
    struct Ltc68111Comm *out) {
    if (handler == NULL || payload == NULL || out == NULL) {
        return 0U;
    }

    size_t decoded = 0, off = 0;
    const size_t byte_count = LTC6811_1_REG_BYTE_COUNT + LTC6811_1_PEC_BYTE_COUNT;
    for (size_t i = 0; i < handler->count; ++i) {
        // Check PEC validity
        if (prv_ltc6811_1_pec_is_correct(payload + off, byte_count)) {
            // Decode communication data
            out[i].icom0 = payload[off + 0] >> 4;
            out[i].icom1 = payload[off + 2] >> 4;
            out[i].icom2 = payload[off + 4] >> 4;
            out[i].fcom0 = payload[off + 1] & 0x0F;
            out[i].fcom1 = payload[off + 3] & 0x0F;
            out[i].fcom2 = payload[off + 5] & 0x0F;
            for (size_t j = 0; j < LTC6811_1_REG_COMM_COUNT; ++j) {
                out[i].payload[j] = ((payload[off + j * 2] & 0x0F) << 4) |
                                    (payload[off + j * 2 + 1] >> 4);
            }

            decoded += byte_count;
        }
        off += byte_count;
    }
    return decoded;
}

size_t ltc6811_1_stcomm_encode_broadcast(
    const struct Ltc68111Handler *handler,
    uint8_t *out) {
    if (handler == NULL || out == NULL) {
        return 0U;
    }

    enum Ltc68111Command cmd = LTC6811_1_CMD_STCOMM;
    size_t encoded = prv_ltc6811_1_cmd_encode(cmd, out);

    // Add dummy data
    memset(out + encoded, 0xff, LTC6811_1_STCOMM_CYCLES);
    encoded += LTC6811_1_STCOMM_CYCLES;
    return encoded;
}
