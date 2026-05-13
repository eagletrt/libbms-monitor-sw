/*!
 * \file            ltc6810-2.h
 * \date            2026-5-13
 * \authors         Mirko Lana [mirko.lana@eagletrt.it]
 *
 * \brief           Constants and data structure definitions needed for the
 *                  LTC6810-2.
 *
 * \details         For additional information refer to the datasheet:
 * \link            https://www.analog.com/media/en/technical-documentation/data-sheets/ltc6810-1-6810-2.pdf
 */

#ifndef LTC6810_2_H
#define LTC6810_2_H

#include <stdint.h>
#include <stddef.h>

#define LTC6810_2_REG_BYTE_COUNT (6U)  /*!< Number of bytes in a single register */
#define LTC6810_2_CMD_BYTE_COUNT (2U)  /*!< Number of bytes of a single command (without PEC) */
#define LTC6810_2_PEC_BYTE_COUNT (2U)  /*!< Number of bytes of the PEC */
#define LTC6810_2_POLL_BYTE_COUNT (1U) /*!< Number of bytes of a single poll response */

#define LTC6810_2_T_IDLE_MS (5U)     /*!< Time required for the isoSPI port to go to the IDLE state in ms */
#define LTC6810_2_T_SLEEP_MS (2000U) /*!< Time required for the LTC6811 to go the SLEEP state in ms */

#define LTC6810_2_STCOMM_CYCLES (72U) /*!< Required number of clock cycles for an STCOMM command completion */

#define LTC6810_2_PLADC_COMPLETE_BYTE_VALUE (0xFF)

/*!
 * \defgroup ltc6810_2_cell_voltage_registers Cell voltage registers definitions
 * @{
 */

#define LTC6810_2_REG_CELL_COUNT (3U)                                          /*!< Number of cell voltages in a single register (C[X]V) */
#define LTC6810_2_CELL_COUNT (LTC6810_2_REG_CELL_COUNT * LTC6810_2_CVXR_COUNT) /*!< Number of cells voltages in all registers (CV) */

/*! @} */

/*!
 * \defgroup ltc6810_2_aux_voltage_registers Auxiliary voltage registers definitions
 * @{
 */

#define LTC6810_2_REG_AUX_COUNT (3U)                                         /*!< Number of auxiliary voltages in a single register (G[X]V) */
#define LTC6810_2_AUX_COUNT (LTC6810_2_REG_AUX_COUNT * LTC6810_2_AVXR_COUNT) /*!< Number of auxiliary voltages in all registers (GV) */

/*! @} */

/*!
 * \defgroup ltc6810_2_s_pin_control_registers S pin control registers definitions
 * @{
 */

#define LTC6810_2_REG_SCTRL_COUNT (12U)                                            /*!< Number of S pin control values in a single register (SCTRL[X]) */
#define LTC6810_2_SCTRL_COUNT (LTC6810_2_REG_SCTRL_COUNT * LTC6810_2_SCTRLX_COUNT) /*!< Number of S pin control values in all registers (SCTRL) */

/*! @} */

/*!
 * \defgroup ltc6810_2_pwm_registers PWM registers definitions
 * @{
 */

#define LTC6810_2_REG_PWM_COUNT (12U)                                         /*!< Number of PWM values in a single register (PWM[X]) */
#define LTC6810_2_PWM_COUNT (LTC6810_2_REG_PWM_COUNT * LTC6810_2_PWMXR_COUNT) /*!< Number of PWM values in all registers (PWM) */

/*! @} */

/*!
 * \defgroup ltc6810_2_comm_registers COMM registers definitions
 * @{
 */

#define LTC6810_2_REG_COMM_COUNT (3U)                                           /*!< Maximum number of bytes that can be written or read from/to a single COMM register (D[X]) */
#define LTC6810_2_COMM_COUNT (LTC6810_2_REG_COMM_COUNT * LTC6810_2_COMMX_COUNT) /*!< Maximum number of bytes that can be written or read from/to all COMM registers (D) */

/*! @} */

/*!
 * \brief           Get the buffer size of an encoded write command in bytes
 *
 * \details         With multiple ICs the command needs to be sent only once
 *                  with all the payloads after it
 *
 * \param[in]       COUNT: The total number of ICs in the chain
 * \return          The buffer size in bytes
 */
#define LTC6810_2_WRITE_BUFFER_SIZE(COUNT) (                  \
    (LTC6810_2_CMD_BYTE_COUNT) + (LTC6810_2_PEC_BYTE_COUNT) + \
    ((LTC6810_2_REG_BYTE_COUNT) + (LTC6810_2_PEC_BYTE_COUNT)) * (COUNT))

/*!
 * \brief           Get the buffer size of an encoded read command in bytes
 */
#define LTC6810_2_READ_BUFFER_SIZE ((LTC6810_2_CMD_BYTE_COUNT) + (LTC6810_2_PEC_BYTE_COUNT))

/*!
 * \brief           Get the buffer size of the data received from the ICs in bytes
 *
 * \param[in]       COUNT: The total number of ICs in the chain
 * \return          The buffer size in bytes
 */
#define LTC6810_2_DATA_BUFFER_SIZE(COUNT) (((LTC6810_2_REG_BYTE_COUNT) + (LTC6810_2_PEC_BYTE_COUNT)) * (COUNT))

/*!
 * \brief           Get the buffer size of an encoded poll command in bytes
 */
#define LTC6810_2_POLL_BUFFER_SIZE ((LTC6810_2_CMD_BYTE_COUNT) + (LTC6810_2_PEC_BYTE_COUNT))

/*!
 * \brief           Get the buffer size of an encoded 'STCOMM' command in bytes
 *
 * \details         After the STCOMM command it is needed to wait 24 clock cycles
 *                  for each byte sent to the slave
 */
#define LTC6810_2_STCOMM_BUFFER_SIZE ((LTC6810_2_CMD_BYTE_COUNT) + (LTC6810_2_PEC_BYTE_COUNT) + (LTC6810_2_STCOMM_CYCLES))

/*!
 * \brief           List of available commands for communication with the LTC6810-2
 *
 * \details         Each command maked with (*) is provided for forward-compatibility with LTC6813/6812
 */
enum Ltc68102Command {
    LTC6810_2_CMD_WRCFGA = 0b00000000001,   /*!<     Write configuration register group A */
    LTC6810_2_CMD_WRCFGB = 0b00000100100,   /*!< (*) Write configuration register group B */
    LTC6810_2_CMD_RDCFGA = 0b00000000010,   /*!<     Read configuration register group A */
    LTC6810_2_CMD_RDCFGB = 0b00000100110,   /*!< (*) Read configuration register group B */
    LTC6810_2_CMD_RDCVA = 0b00000000100,    /*!<     Read cell voltage register group A */
    LTC6810_2_CMD_RDCVB = 0b00000000110,    /*!<     Read cell voltage register group B */
    LTC6810_2_CMD_RDCVC = 0b00000001000,    /*!<     Read cell voltage register group C */
    LTC6810_2_CMD_RDCVD = 0b00000001010,    /*!<     Read cell voltage register group D */
    LTC6810_2_CMD_RDCVE = 0b00000001001,    /*!< (*) Read cell voltage register group E */
    LTC6810_2_CMD_RDCVF = 0b00000001011,    /*!< (*) Read cell voltage register group F */
    LTC6810_2_CMD_RDAUXA = 0b00000001100,   /*!<     Read auxiliary register group A */
    LTC6810_2_CMD_RDAUXB = 0b00000001110,   /*!<     Read auxiliary register group B */
    LTC6810_2_CMD_RDAUXC = 0b00000001101,   /*!< (*) Read auxiliary register group C */
    LTC6810_2_CMD_RDAUXD = 0b00000001111,   /*!< (*) Read auxiliary register group D */
    LTC6810_2_CMD_RDSTATA = 0b00000010000,  /*!<     Read status register group A */
    LTC6810_2_CMD_RDSTATB = 0b00000010010,  /*!<     Read status register group B */
    LTC6810_2_CMD_WRSCTRL = 0b00000010100,  /*!<     Write S control register group */
    LTC6810_2_CMD_WRPWM = 0b00000100000,    /*!<     Write PWM register group */
    LTC6810_2_CMD_WRPSB = 0b00000011100,    /*!< (*) Write PWM/S control register group B */
    LTC6810_2_CMD_RDSCTRL = 0b00000010110,  /*!<     Read S control register group */
    LTC6810_2_CMD_RDPWM = 0b00000100010,    /*!<     Read PWM register group */
    LTC6810_2_CMD_RDPSB = 0b00000011110,    /*!< (*) Read PWM/S control register group B */
    LTC6810_2_CMD_STSCTRL = 0b00000011001,  /*!<     Start S control pulsing and poll status */
    LTC6810_2_CMD_CLRSCTRL = 0b00000011000, /*!<     Clear S control register group */
    LTC6810_2_CMD_ADCV = 0b01001100000,     /*!<     Start cell voltage ADC conversion and poll status */
    LTC6810_2_CMD_ADOW = 0b01000101000,     /*!<     Start open wire ADC conversion and poll status */
    LTC6810_2_CMD_CVST = 0b01000000111,     /*!<     Start self test cell voltage conversion and poll status */
    LTC6810_2_CMD_ADOL = 0b01000000001,     /*!<     Start overlap measurement of cell 7 voltage */
    LTC6810_2_CMD_ADAX = 0b10001100000,     /*!<     Start GPIOs ADC conversion and poll status */
    LTC6810_2_CMD_ADAXD = 0b10000000000,    /*!<     Start GPIOs ADC with digital redundancy and poll status */
    LTC6810_2_CMD_AXST = 0b10000000111,     /*!<     Start self test GPIOs conversion and poll status */
    LTC6810_2_CMD_ADSTAT = 0b10001101000,   /*!<     Start status group ADC conversion and poll status */
    LTC6810_2_CMD_ADSTATD = 0b10000001000,  /*!<     Start status group ADC conversion with digital redundancy and poll status */
    LTC6810_2_CMD_STATST = 0b10000001111,   /*!<     Start self test status group conversion and poll status */
    LTC6810_2_CMD_ADCVAX = 0b10001101111,   /*!<     Start combined cell voltage and GPIO1, GPIO2 conversion and poll status */
    LTC6810_2_CMD_ADCVSC = 0b10001100111,   /*!<     Start combined cell voltage and SC conversion and poll status */
    LTC6810_2_CMD_CLRCELL = 0b11100010001,  /*!<     Clear cell voltage register groups */
    LTC6810_2_CMD_CLRAUX = 0b11100010010,   /*!<     Clear auxiliary register groups */
    LTC6810_2_CMD_CLRSTAT = 0b11100010011,  /*!<     Clear status register groups */
    LTC6810_2_CMD_PLADC = 0b11100010100,    /*!<     Poll ADC conversion status */
    LTC6810_2_CMD_DIAGN = 0b11100010101,    /*!<     Diagnose MUX and poll status */
    LTC6810_2_CMD_WRCOMM = 0b11100100001,   /*!<     Write COMM register group */
    LTC6810_2_CMD_RDCOMM = 0b11100100010,   /*!<     Read COMM register group */
    LTC6810_2_CMD_STCOMM = 0b11100100011    /*!<     Start I2C/SPI communication */
};

/*!
 * \brief           Configuration register group names
 */
enum Ltc68102Cfgxr {
    LTC6810_2_CFGAR = 0,
    LTC6810_2_CFGXR_COUNT
};

/*!
 * \brief           Cell voltage register group names
 */
enum Ltc68102Cvxr {
    LTC6810_2_CVAR = 0,
    LTC6810_2_CVBR,
    LTC6810_2_CVCR,
    LTC6810_2_CVDR,
    LTC6810_2_CVXR_COUNT
};

/*!
 * \brief           Auxiliary voltage register group names
 */
enum Ltc68102Avxr {
    LTC6810_2_AVAR = 0,
    LTC6810_2_AVBR,
    LTC6810_2_AVXR_COUNT
};

/*!
 * \brief           Status register group names
 */
enum Ltc68102Stxr {
    LTC6810_2_STAR = 0,
    LTC6810_2_STBR,
    LTC6810_2_STXR_COUNT
};

/*!
 * \brief           COMM register group names
 */
enum Ltc68102Commx {
    LTC6810_2_COMMA = 0,
    LTC6810_2_COMMX_COUNT
};

/*!
 * \brief           S pin control register group names
 */
enum Ltc68102Sctrlx {
    LTC6810_2_SCTRLA = 0,
    LTC6810_2_SCTRLX_COUNT
};

/*!
 * \brief           PWM register group names
 */
enum Ltc68102Pwmxr {
    LTC6810_2_PWMAR = 0,
    LTC6810_2_PWMXR_COUNT
};

/*!
 * \brief           ADC conversion mode
 *
 * \warning         Mode selection depends on ADCOPT flags in the IC
 *                  configuration register:
 *                      ADCOPT = 0 -> selects modes 26Hz, 422Hz, 7kHz or 27kHz (default)
 *                      ADCOPT = 1 -> selects modes 1kHz, 2kHz, 3kHz or 14kHz
 */
enum Ltc68102Md {
    LTC6810_2_MD_422HZ = 0,
    LTC6810_2_MD_27KHZ,
    LTC6810_2_MD_7KHZ,
    LTC6810_2_MD_26HZ,
    LTC6810_2_MD_1KHZ = LTC6810_2_MD_422HZ,
    LTC6810_2_MD_14KHZ = LTC6810_2_MD_27KHZ,
    LTC6810_2_MD_3KHZ = LTC6810_2_MD_7KHZ,
    LTC6810_2_MD_2KHZ = LTC6810_2_MD_26HZ,
    LTC6810_2_MD_COUNT = 4
};

/*!
 * \brief           Pull-Up/Pull-Down current selection for Open Wire conversions
 */
enum Ltc68102Pup {
    LTC6810_2_PUP_INACTIVE = 0, /*!< Pull-Down current */
    LTC6810_2_PUP_ACTIVE,       /*!< Pull-Up current */
    LTC6810_2_PUP_COUNT
};

/*!
 * \brief           Self test mode selection
 */
enum Ltc68102St {
    LTC6810_2_ST_ONE = 1,
    LTC6810_2_ST_TWO,
    LTC6810_2_ST_COUNT
};

/*!
 * \brief           Option to permit or deny measurement during discharge
 */
enum Ltc68102Dcp {
    LTC6810_2_DCP_DISABLED = 0, /*!< Discharge not permitted */
    LTC6810_2_DCP_ENABLED,      /*!< Discharge permitted */
    LTC6810_2_DCP_COUNT
};

/*!
 * \brief          Cell selection for ADC conversion
 */
enum Ltc68102Ch {
    LTC6810_2_CH_ALL = 0,
    LTC6810_2_CH_1,
    LTC6810_2_CH_2,
    LTC6810_2_CH_3,
    LTC6810_2_CH_4,
    LTC6810_2_CH_5,
    LTC6810_2_CH_6,
    LTC6810_2_CH_7 = LTC6810_2_CH_1,
    LTC6810_2_CH_8 = LTC6810_2_CH_2,
    LTC6810_2_CH_9 = LTC6810_2_CH_3,
    LTC6810_2_CH_10 = LTC6810_2_CH_4,
    LTC6810_2_CH_11 = LTC6810_2_CH_5,
    LTC6810_2_CH_12 = LTC6810_2_CH_6,
    LTC6810_2_CH_COUNT = 7
};

/*!
 * \brief           GPIO selection for ADC conversion
 */
enum Ltc68102Chg {
    LTC6810_2_CHG_GPIO_ALL = 0,
    LTC6810_2_CHG_GPIO_1,
    LTC6810_2_CHG_GPIO_2,
    LTC6810_2_CHG_GPIO_3,
    LTC6810_2_CHG_GPIO_4,
    LTC6810_2_CHG_GPIO_5,
    LTC6810_2_CHG_SECOND_REF, /*!< Second voltage reference */
    LTC6810_2_CHG_COUNT
};

/*!
 * \brief           Status group selection
 */
enum Ltc68102Chst {
    LTC6810_2_CHST_ALL = 0,
    LTC6810_2_CHST_SC,   /*!< Sum of all cells voltages */
    LTC6810_2_CHST_ITMP, /*!< Internal Die Temperature */
    LTC6810_2_CHST_VA,   /*!< Analog power supply */
    LTC6810_2_CHST_VD,   /*!< Digital power supply */
    LTC6810_2_CHST_COUNT
};

/*!
 * \brief           Discharge timeout selection values
 */
enum Ltc68102Dcto {
    LTC6810_2_DCTO_OFF = 0, /*!< Discharge is disabled */
    LTC6810_2_DCTO_30S,
    LTC6810_2_DCTO_1MIN,
    LTC6810_2_DCTO_2MIN,
    LTC6810_2_DCTO_3MIN,
    LTC6810_2_DCTO_4MIN,
    LTC6810_2_DCTO_5MIN,
    LTC6810_2_DCTO_10MIN,
    LTC6810_2_DCTO_15MIN,
    LTC6810_2_DCTO_20MIN,
    LTC6810_2_DCTO_30MIN,
    LTC6810_2_DCTO_40MIN,
    LTC6810_2_DCTO_60MIN,
    LTC6810_2_DCTO_75MIN,
    LTC6810_2_DCTO_90MIN,
    LTC6810_2_DCTO_120MIN
};

/*!
 * \brief           I2C inital communication control bits on write
 */
enum Ltc68102I2cWicom {
    LTC6810_2_I2C_WICOM_START = 0b0110,     /*!< Start signal that will be followed by data */
    LTC6810_2_I2C_WICOM_STOP = 0b0001,      /*!< Stop signal */
    LTC6810_2_I2C_WICOM_BLANK = 0b0000,     /*!< Proceed to data trasmission */
    LTC6810_2_I2C_WICOM_NOTRANSMIT = 0b0111 /*!< Release SDA and SCL and ignore the rest of data */
};

/*!
 * \brief           I2C final communication control bits on write
 */
enum Ltc68102I2cWfcom {
    LTC6810_2_I2C_WFCOM_ACK = 0b0000,  /*!< Generate an ACK signal on ninth clock cycle */
    LTC6810_2_I2C_WFCOM_NACK = 0b1000, /*!< Generate a NACK signal on ninth clock cycle */
    LTC6810_2_I2C_WFCOM_STOP = 0b1001, /*!< Generate a NACK signal followed by STOP signal */
};

/*!
 * \brief           SPI inital communication control bits on write
 */
enum Ltc68102SpiWicom {
    LTC6810_2_SPI_WICOM_LOW = 0b1000,        /*!< Generate a CSBM low signal on SPI port */
    LTC6810_2_SPI_WICOM_FALLING = 0b1010,    /*!< Drive CSBM high and then low */
    LTC6810_2_SPI_WICOM_HIGH = 0b1001,       /*!< Generate a CSBM high signal on SPI port */
    LTC6810_2_SPI_WICOM_NOTRANSMIT = 0b1111, /*!< Release the SPI port and ignore the rest of the data */
};

/*!
 * \brief           SPI final communication control bits on write
 *
 * \warning         The low value can be either 0b0000 or 0b1000
 */
enum Ltc68102SpiWfcom {
    LTC6810_2_SPI_WFCOM_LOW = 0b0000,   /*!< Holds CSBM low at the end of byte transmission */
    LTC6810_2_SPI_WFCOM_LOW_B = 0b1000, /*!< Holds CSBM low at the end of byte transmission */
    LTC6810_2_SPI_WFCOM_HIGH = 0b1001   /*!< Transitions CSBM high at the end of byte transmission */
};

/*!
 * \brief           I2C inital communication control bits on read
 */
enum Ltc68102I2cRicom {
    LTC6811_I2C_RICOM_START = 0b0110,     /*!< Master generated a start signal */
    LTC6811_I2C_RICOM_STOP = 0b0001,      /*!< Master generated a stop signal */
    LTC6811_I2C_RICOM_BLANK = 0b0000,     /*!< SDA was held low between bytes */
    LTC6811_I2C_RICOM_NOTRANSMIT = 0b0111 /*!< SDA was held high between bytes */
};

/*!
 * \brief           I2C final communication control bits on read
 */
enum Ltc68102I2cRfcom {
    LTC6810_2_I2C_RFCOM_MACK = 0b0000,      /*!< Master generated an ACK signal */
    LTC6810_2_I2C_RFCOM_SACK = 0b0111,      /*!< Slave generated an ACK signal */
    LTC6810_2_I2C_RFCOM_SNACK = 0b1111,     /*!< Slave generated a NACK signal */
    LTC6810_2_I2C_RFCOM_SACK_STOP = 0b0001, /*!< Slave generated an ACK and master generated a STOP signal */
    LTC6810_2_I2C_RFCOM_SNACK_STOP = 0b1001 /*!< Slave generated a NACK and master generated a STOP signal */
};

/*!
 * \brief           SPI inital communication control bits on read
 */
enum Ltc68102SpiRicom {
    LTC6810_2_SPI_RICOM = 0b0111
};

/*!
 * \brief           SPI final communication control bits on read
 */
enum Ltc68102SpiRfcom {
    LTC6810_2_SPI_RFCOM = 0b1111
};

/*!
 * \brief           Configuration register group structure
 */
struct Ltc68102Cfgr {
    uint8_t ADCOPT : 1; /*!< ADC mode option bit */
    uint8_t DTEN : 1;   /*!< Discharge timer enable (READ ONLY) */
    uint8_t REFON : 1;  /*!< References powered up */
    uint8_t GPIO : 5;   /*!< GPIOx pin control (GPIO1 starts from the least significant bit) */
    uint16_t VUV : 12;  /*!< Undervoltage comparison voltage */
    uint16_t VOV : 12;  /*!< Overvoltage comparison voltage */
    uint16_t DCC : 12;  /*!< Discharge cell x */
    uint8_t DCTO : 4;   /*!< Discharge time out value */
} __attribute__((__packed__));

/*!
 * \brief           Status register group structure
 */
struct Ltc68102Str {
    uint16_t SC;         /*!< Sum of all cells measurement */
    uint16_t ITMP;       /*!< Internal die temperature */
    uint16_t VA;         /*!< Analog power supply voltage */
    uint16_t VD;         /*!< Digital power supply voltage */
    uint16_t COV : 12;   /*!< Cell over voltage flag */
    uint16_t CUV : 12;   /*!< Cell under voltage flag */
    uint8_t REV : 4;     /*!< Device revision code */
    uint8_t RSVD : 2;    /*!< Reserved bits (always 0s) */
    uint8_t MUXFAIL : 1; /*!< Multiplexer self test result */
    uint8_t THSD : 1;    /*!< Thermal shutdown status */
} __attribute__((__packed__));

/*!
 * \brief           External communication register group structure
 *
 * \details         The IC can be used as master for I2C and SPI communication
 *                  via its GPIOs.
 *                  For I2C:
 *                      - GPIO4 is used as the Serial DAta(SDA)
 *                      - GPIO5 is used as the Serial CLock(SCL)
 *                  For SPI:
 *                      - GPIO3 is used as the Chip Select(CSBM)
 *                      - GPIO4 is used as the Serial Data Input-Output(SDIOM)
 *                      - GPIO5 is used as the Serial Clock(SCKM)
 *
 * \warning         For SPI only mode 3 (CHPA = 1, CPOL = 1) is supported
 */
struct Ltc68102Comm {
    uint8_t icom0 : 4;                     /*!< Initial communication control bits of the first data byte */
    uint8_t icom1 : 4;                     /*!< Initial communication control bits of the second data byte */
    uint8_t icom2 : 4;                     /*!< Initial communication control bits of the third data byte */
    uint8_t fcom0 : 4;                     /*!< Final communication control bits of the first data byte */
    uint8_t fcom1 : 4;                     /*!< Final communication control bits of the second data byte */
    uint8_t fcom2 : 4;                     /*!< Final communication control bits of the third data byte */
    uint8_t payload[LTC6810_2_COMM_COUNT]; /*!< Payload data transmited (received) to (from) I2C/SPI slave device */
} __attribute__((__packed__));

/*!
 * \brief           LTC6810-2 handler structure
 */
struct Ltc68102Handler {
    size_t count;
};

#endif
