/*!
 * \file            ltc6811-1.h
 * \date            2023-12-16
 * \authors         Antonio Gelain [antonio.gelain2@gmail.com]
 *                  Riccardo Segala [riccardo.segala@eagletrt.it]
 *
 * \brief           Constants and data structure definitions needed for the
 *                  LTC6811-1.
 *
 * \details         For additional information refer to the datasheet:
 * \link            https://www.analog.com/media/en/technical-documentation/data-sheets/ltc6811-1-6811-2.pdf
 *
 * \warning         For SPI communication with the LTC6811-1, mode 3 is
 *                  required (in CubeMX is CPOL="Low" and CPHA="2 edge") and
 *                  the MISO pin has to be set in Pull-Up mode.
 *                  Maximum SPI communication speed with the LTC is 1MHz.
 */

#ifndef LTC6811_1_H
#define LTC6811_1_H

#include <stdint.h>
#include <stddef.h>

#define LTC6811_1_REG_BYTE_COUNT (6U)  /*!< Number of bytes in a single register */
#define LTC6811_1_CMD_BYTE_COUNT (2U)  /*!< Number of bytes of a single command (without PEC) */
#define LTC6811_1_PEC_BYTE_COUNT (2U)  /*!< Number of bytes of the PEC */
#define LTC6811_1_POLL_BYTE_COUNT (1U) /*!< Number of bytes of a single poll response */

#define LTC6811_1_T_IDLE_MS (5U)     /*!< Time required for the isoSPI port to go to the IDLE state in ms */
#define LTC6811_1_T_SLEEP_MS (2000U) /*!< Time required for the LTC6811 to go the SLEEP state in ms */

#define LTC6811_1_STCOMM_CYCLES (72U) /*!< Required number of clock cycles for an STCOMM command completion */

#define LTC6811_1_PLADC_COMPLETE_BYTE_VALUE (0xFF) /*!< Byte returned by the LTC6811-1 whenever the ADC conversion is completed */

#define LTC6811_1_OPEN_WIRE_THRESHOLD_V (-0.4F) /*!< Byte returned by the LTC6811-1 whenever the ADC conversion is completed */

/*!
 * \defgroup ltc6811_1_cell_voltage_registers Cell voltage registers definitions
 * @{
 */

#define LTC6811_1_REG_CELL_COUNT (3U)                                          /*!< Number of cell voltages in a single register (C[X]V) */
#define LTC6811_1_CELL_COUNT (LTC6811_1_REG_CELL_COUNT * LTC6811_1_CVXR_COUNT) /*!< Number of cells voltages in all registers (CV) */

/*! @} */

/*!
 * \defgroup ltc6811_1_aux_voltage_registers Auxiliary voltage registers definitions
 * @{
 */

#define LTC6811_1_REG_AUX_COUNT (3U)                                         /*!< Number of auxiliary voltages in a single register (G[X]V) */
#define LTC6811_1_AUX_COUNT (LTC6811_1_REG_AUX_COUNT * LTC6811_1_AVXR_COUNT) /*!< Number of auxiliary voltages in all registers (GV) */

/*! @} */

/*!
 * \defgroup ltc6811_1_s_pin_control_registers S pin control registers definitions
 * @{
 */

#define LTC6811_1_REG_SCTRL_COUNT (12U)                                            /*!< Number of S pin control values in a single register (SCTRL[X]) */
#define LTC6811_1_SCTRL_COUNT (LTC6811_1_REG_SCTRL_COUNT * LTC6811_1_SCTRLX_COUNT) /*!< Number of S pin control values in all registers (SCTRL) */

/*! @} */

/*!
 * \defgroup ltc6811_1_pwm_registers PWM registers definitions
 * @{
 */

#define LTC6811_1_REG_PWM_COUNT (12U)                                         /*!< Number of PWM values in a single register (PWM[X]) */
#define LTC6811_1_PWM_COUNT (LTC6811_1_REG_PWM_COUNT * LTC6811_1_PWMXR_COUNT) /*!< Number of PWM values in all registers (PWM) */

/*! @} */

/*!
 * \defgroup ltc6811_1_comm_registers COMM registers definitions
 * @{
 */

#define LTC6811_1_REG_COMM_COUNT (3U)                                           /*!< Maximum number of bytes that can be written or read from/to a single COMM register (D[X]) */
#define LTC6811_1_COMM_COUNT (LTC6811_1_REG_COMM_COUNT * LTC6811_1_COMMX_COUNT) /*!< Maximum number of bytes that can be written or read from/to all COMM registers (D) */

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
#define LTC6811_1_WRITE_BUFFER_SIZE(COUNT) (                  \
    (LTC6811_1_CMD_BYTE_COUNT) + (LTC6811_1_PEC_BYTE_COUNT) + \
    ((LTC6811_1_REG_BYTE_COUNT) + (LTC6811_1_PEC_BYTE_COUNT)) * (COUNT))

/*!
 * \brief           Get the buffer size of an encoded read command in bytes
 */
#define LTC6811_1_READ_BUFFER_SIZE ((LTC6811_1_CMD_BYTE_COUNT) + (LTC6811_1_PEC_BYTE_COUNT))

/*!
 * \brief           Get the buffer size of the data received from the ICs in bytes
 *
 * \param[in]       COUNT: The total number of ICs in the chain
 * \return          The buffer size in bytes
 */
#define LTC6811_1_DATA_BUFFER_SIZE(COUNT) (((LTC6811_1_REG_BYTE_COUNT) + (LTC6811_1_PEC_BYTE_COUNT)) * (COUNT))

/*!
 * \brief           Get the buffer size of an encoded poll command in bytes
 */
#define LTC6811_1_POLL_BUFFER_SIZE ((LTC6811_1_CMD_BYTE_COUNT) + (LTC6811_1_PEC_BYTE_COUNT))

/*!
 * \brief           Get the buffer size of an encoded 'STCOMM' command in bytes
 *
 * \details         After the STCOMM command it is needed to wait 24 clock cycles
 *                  for each byte sent to the slave
 */
#define LTC6811_1_STCOMM_BUFFER_SIZE ((LTC6811_1_CMD_BYTE_COUNT) + (LTC6811_1_PEC_BYTE_COUNT) + (LTC6811_1_STCOMM_CYCLES))

/*!
 * \brief           List of available commands for communication with the LTC6811-1
 *
 * \details         Each command maked with (*) is provided for forward-compatibility with LTC6813/6812
 */
enum Ltc68111Command {
    LTC6811_1_CMD_WRCFGA = 0b00000000001,   /*!<     Write configuration register group A */
    LTC6811_1_CMD_WRCFGB = 0b00000100100,   /*!< (*) Write configuration register group B */
    LTC6811_1_CMD_RDCFGA = 0b00000000010,   /*!<     Read configuration register group A */
    LTC6811_1_CMD_RDCFGB = 0b00000100110,   /*!< (*) Read configuration register group B */
    LTC6811_1_CMD_RDCVA = 0b00000000100,    /*!<     Read cell voltage register group A */
    LTC6811_1_CMD_RDCVB = 0b00000000110,    /*!<     Read cell voltage register group B */
    LTC6811_1_CMD_RDCVC = 0b00000001000,    /*!<     Read cell voltage register group C */
    LTC6811_1_CMD_RDCVD = 0b00000001010,    /*!<     Read cell voltage register group D */
    LTC6811_1_CMD_RDCVE = 0b00000001001,    /*!< (*) Read cell voltage register group E */
    LTC6811_1_CMD_RDCVF = 0b00000001011,    /*!< (*) Read cell voltage register group F */
    LTC6811_1_CMD_RDAUXA = 0b00000001100,   /*!<     Read auxiliary register group A */
    LTC6811_1_CMD_RDAUXB = 0b00000001110,   /*!<     Read auxiliary register group B */
    LTC6811_1_CMD_RDAUXC = 0b00000001101,   /*!< (*) Read auxiliary register group C */
    LTC6811_1_CMD_RDAUXD = 0b00000001111,   /*!< (*) Read auxiliary register group D */
    LTC6811_1_CMD_RDSTATA = 0b00000010000,  /*!<     Read status register group A */
    LTC6811_1_CMD_RDSTATB = 0b00000010010,  /*!<     Read status register group B */
    LTC6811_1_CMD_WRSCTRL = 0b00000010100,  /*!<     Write S control register group */
    LTC6811_1_CMD_WRPWM = 0b00000100000,    /*!<     Write PWM register group */
    LTC6811_1_CMD_WRPSB = 0b00000011100,    /*!< (*) Write PWM/S control register group B */
    LTC6811_1_CMD_RDSCTRL = 0b00000010110,  /*!<     Read S control register group */
    LTC6811_1_CMD_RDPWM = 0b00000100010,    /*!<     Read PWM register group */
    LTC6811_1_CMD_RDPSB = 0b00000011110,    /*!< (*) Read PWM/S control register group B */
    LTC6811_1_CMD_STSCTRL = 0b00000011001,  /*!<     Start S control pulsing and poll status */
    LTC6811_1_CMD_CLRSCTRL = 0b00000011000, /*!<     Clear S control register group */
    LTC6811_1_CMD_ADCV = 0b01001100000,     /*!<     Start cell voltage ADC conversion and poll status */
    LTC6811_1_CMD_ADOW = 0b01000101000,     /*!<     Start open wire ADC conversion and poll status */
    LTC6811_1_CMD_CVST = 0b01000000111,     /*!<     Start self test cell voltage conversion and poll status */
    LTC6811_1_CMD_ADOL = 0b01000000001,     /*!<     Start overlap measurement of cell 7 voltage */
    LTC6811_1_CMD_ADAX = 0b10001100000,     /*!<     Start GPIOs ADC conversion and poll status */
    LTC6811_1_CMD_ADAXD = 0b10000000000,    /*!<     Start GPIOs ADC with digital redundancy and poll status */
    LTC6811_1_CMD_AXST = 0b10000000111,     /*!<     Start self test GPIOs conversion and poll status */
    LTC6811_1_CMD_ADSTAT = 0b10001101000,   /*!<     Start status group ADC conversion and poll status */
    LTC6811_1_CMD_ADSTATD = 0b10000001000,  /*!<     Start status group ADC conversion with digital redundancy and poll status */
    LTC6811_1_CMD_STATST = 0b10000001111,   /*!<     Start self test status group conversion and poll status */
    LTC6811_1_CMD_ADCVAX = 0b10001101111,   /*!<     Start combined cell voltage and GPIO1, GPIO2 conversion and poll status */
    LTC6811_1_CMD_ADCVSC = 0b10001100111,   /*!<     Start combined cell voltage and SC conversion and poll status */
    LTC6811_1_CMD_CLRCELL = 0b11100010001,  /*!<     Clear cell voltage register groups */
    LTC6811_1_CMD_CLRAUX = 0b11100010010,   /*!<     Clear auxiliary register groups */
    LTC6811_1_CMD_CLRSTAT = 0b11100010011,  /*!<     Clear status register groups */
    LTC6811_1_CMD_PLADC = 0b11100010100,    /*!<     Poll ADC conversion status */
    LTC6811_1_CMD_DIAGN = 0b11100010101,    /*!<     Diagnose MUX and poll status */
    LTC6811_1_CMD_WRCOMM = 0b11100100001,   /*!<     Write COMM register group */
    LTC6811_1_CMD_RDCOMM = 0b11100100010,   /*!<     Read COMM register group */
    LTC6811_1_CMD_STCOMM = 0b11100100011    /*!<     Start I2C/SPI communication */
};

/*!
 * \brief           Configuration register group names
 */
enum Ltc68111Cfgxr {
    LTC6811_1_CFGAR = 0,
    LTC6811_1_CFGXR_COUNT
};

/*!
 * \brief           Cell voltage register group names
 */
enum Ltc68111Cvxr {
    LTC6811_1_CVAR = 0,
    LTC6811_1_CVBR,
    LTC6811_1_CVCR,
    LTC6811_1_CVDR,
    LTC6811_1_CVXR_COUNT
};

/*!
 * \brief           Auxiliary voltage register group names
 */
enum Ltc68111Avxr {
    LTC6811_1_AVAR = 0,
    LTC6811_1_AVBR,
    LTC6811_1_AVXR_COUNT
};

/*!
 * \brief           Status register group names
 */
enum Ltc68111Stxr {
    LTC6811_1_STAR = 0,
    LTC6811_1_STBR,
    LTC6811_1_STXR_COUNT
};

/*!
 * \brief           COMM register group names
 */
enum Ltc68111Commx {
    LTC6811_1_COMMA = 0,
    LTC6811_1_COMMX_COUNT
};

/*!
 * \brief           S pin control register group names
 */
enum Ltc68111Sctrlx {
    LTC6811_1_SCTRLA = 0,
    LTC6811_1_SCTRLX_COUNT
};

/*!
 * \brief           PWM register group names
 */
enum Ltc68111Pwmxr {
    LTC6811_1_PWMAR = 0,
    LTC6811_1_PWMXR_COUNT
};

/*!
 * \brief           ADC conversion mode
 *
 * \warning         Mode selection depends on ADCOPT flags in the IC
 *                  configuration register:
 *                      ADCOPT = 0 -> selects modes 26Hz, 422Hz, 7kHz or 27kHz (default)
 *                      ADCOPT = 1 -> selects modes 1kHz, 2kHz, 3kHz or 14kHz
 */
enum Ltc68111Md {
    LTC6811_1_MD_422HZ = 0,
    LTC6811_1_MD_27KHZ,
    LTC6811_1_MD_7KHZ,
    LTC6811_1_MD_26HZ,
    LTC6811_1_MD_1KHZ = LTC6811_1_MD_422HZ,
    LTC6811_1_MD_14KHZ = LTC6811_1_MD_27KHZ,
    LTC6811_1_MD_3KHZ = LTC6811_1_MD_7KHZ,
    LTC6811_1_MD_2KHZ = LTC6811_1_MD_26HZ,
    LTC6811_1_MD_COUNT = 4
};

/*!
 * \brief           Pull-Up/Pull-Down current selection for Open Wire conversions
 */
enum Ltc68111Pup {
    LTC6811_1_PUP_INACTIVE = 0, /*!< Pull-Down current */
    LTC6811_1_PUP_ACTIVE,       /*!< Pull-Up current */
    LTC6811_1_PUP_COUNT
};

/*!
 * \brief           Self test mode selection
 */
enum Ltc68111St {
    LTC6811_1_ST_ONE = 1,
    LTC6811_1_ST_TWO,
    LTC6811_1_ST_COUNT
};

/*!
 * \brief           Option to permit or deny measurement during discharge
 */
enum Ltc68111Dcp {
    LTC6811_1_DCP_DISABLED = 0, /*!< Discharge not permitted */
    LTC6811_1_DCP_ENABLED,      /*!< Discharge permitted */
    LTC6811_1_DCP_COUNT
};

/*!
 * \brief          Cell selection for ADC conversion
 */
enum Ltc68111Ch {
    LTC6811_1_CH_ALL = 0,
    LTC6811_1_CH_1,
    LTC6811_1_CH_2,
    LTC6811_1_CH_3,
    LTC6811_1_CH_4,
    LTC6811_1_CH_5,
    LTC6811_1_CH_6,
    LTC6811_1_CH_7 = LTC6811_1_CH_1,
    LTC6811_1_CH_8 = LTC6811_1_CH_2,
    LTC6811_1_CH_9 = LTC6811_1_CH_3,
    LTC6811_1_CH_10 = LTC6811_1_CH_4,
    LTC6811_1_CH_11 = LTC6811_1_CH_5,
    LTC6811_1_CH_12 = LTC6811_1_CH_6,
    LTC6811_1_CH_COUNT = 7
};

/*!
 * \brief           GPIO selection for ADC conversion
 */
enum Ltc68111Chg {
    LTC6811_1_CHG_GPIO_ALL = 0,
    LTC6811_1_CHG_GPIO_1,
    LTC6811_1_CHG_GPIO_2,
    LTC6811_1_CHG_GPIO_3,
    LTC6811_1_CHG_GPIO_4,
    LTC6811_1_CHG_GPIO_5,
    LTC6811_1_CHG_SECOND_REF, /*!< Second voltage reference */
    LTC6811_1_CHG_COUNT
};

/*!
 * \brief           Status group selection
 */
enum Ltc68111Chst {
    LTC6811_1_CHST_ALL = 0,
    LTC6811_1_CHST_SC,   /*!< Sum of all cells voltages */
    LTC6811_1_CHST_ITMP, /*!< Internal Die Temperature */
    LTC6811_1_CHST_VA,   /*!< Analog power supply */
    LTC6811_1_CHST_VD,   /*!< Digital power supply */
    LTC6811_1_CHST_COUNT
};

/*!
 * \brief           Discharge timeout selection values
 */
enum Ltc68111Dcto {
    LTC6811_1_DCTO_OFF = 0, /*!< Discharge is disabled */
    LTC6811_1_DCTO_30S,
    LTC6811_1_DCTO_1MIN,
    LTC6811_1_DCTO_2MIN,
    LTC6811_1_DCTO_3MIN,
    LTC6811_1_DCTO_4MIN,
    LTC6811_1_DCTO_5MIN,
    LTC6811_1_DCTO_10MIN,
    LTC6811_1_DCTO_15MIN,
    LTC6811_1_DCTO_20MIN,
    LTC6811_1_DCTO_30MIN,
    LTC6811_1_DCTO_40MIN,
    LTC6811_1_DCTO_60MIN,
    LTC6811_1_DCTO_75MIN,
    LTC6811_1_DCTO_90MIN,
    LTC6811_1_DCTO_120MIN
};

/*!
 * \brief           I2C inital communication control bits on write
 */
enum Ltc68111I2cWicom {
    LTC6811_1_I2C_WICOM_START = 0b0110,     /*!< Start signal that will be followed by data */
    LTC6811_1_I2C_WICOM_STOP = 0b0001,      /*!< Stop signal */
    LTC6811_1_I2C_WICOM_BLANK = 0b0000,     /*!< Proceed to data trasmission */
    LTC6811_1_I2C_WICOM_NOTRANSMIT = 0b0111 /*!< Release SDA and SCL and ignore the rest of data */
};

/*!
 * \brief           I2C final communication control bits on write
 */
enum Ltc68111I2cWfcom {
    LTC6811_1_I2C_WFCOM_ACK = 0b0000,  /*!< Generate an ACK signal on ninth clock cycle */
    LTC6811_1_I2C_WFCOM_NACK = 0b1000, /*!< Generate a NACK signal on ninth clock cycle */
    LTC6811_1_I2C_WFCOM_STOP = 0b1001, /*!< Generate a NACK signal followed by STOP signal */
};

/*!
 * \brief           SPI inital communication control bits on write
 */
enum Ltc68111SpiWicom {
    LTC6811_1_SPI_WICOM_LOW = 0b1000,        /*!< Generate a CSBM low signal on SPI port */
    LTC6811_1_SPI_WICOM_FALLING = 0b1010,    /*!< Drive CSBM high and then low */
    LTC6811_1_SPI_WICOM_HIGH = 0b1001,       /*!< Generate a CSBM high signal on SPI port */
    LTC6811_1_SPI_WICOM_NOTRANSMIT = 0b1111, /*!< Release the SPI port and ignore the rest of the data */
};

/*!
 * \brief           SPI final communication control bits on write
 *
 * \warning         The low value can be either 0b0000 or 0b1000
 */
enum Ltc68111SpiWfcom {
    LTC6811_1_SPI_WFCOM_LOW = 0b0000,   /*!< Holds CSBM low at the end of byte transmission */
    LTC6811_1_SPI_WFCOM_LOW_B = 0b1000, /*!< Holds CSBM low at the end of byte transmission */
    LTC6811_1_SPI_WFCOM_HIGH = 0b1001   /*!< Transitions CSBM high at the end of byte transmission */
};

/*!
 * \brief           I2C inital communication control bits on read
 */
enum Ltc68111I2cRicom {
    LTC6811_I2C_RICOM_START = 0b0110,     /*!< Master generated a start signal */
    LTC6811_I2C_RICOM_STOP = 0b0001,      /*!< Master generated a stop signal */
    LTC6811_I2C_RICOM_BLANK = 0b0000,     /*!< SDA was held low between bytes */
    LTC6811_I2C_RICOM_NOTRANSMIT = 0b0111 /*!< SDA was held high between bytes */
};

/*!
 * \brief           I2C final communication control bits on read
 */
enum Ltc68111I2cRfcom {
    LTC6811_1_I2C_RFCOM_MACK = 0b0000,      /*!< Master generated an ACK signal */
    LTC6811_1_I2C_RFCOM_SACK = 0b0111,      /*!< Slave generated an ACK signal */
    LTC6811_1_I2C_RFCOM_SNACK = 0b1111,     /*!< Slave generated a NACK signal */
    LTC6811_1_I2C_RFCOM_SACK_STOP = 0b0001, /*!< Slave generated an ACK and master generated a STOP signal */
    LTC6811_1_I2C_RFCOM_SNACK_STOP = 0b1001 /*!< Slave generated a NACK and master generated a STOP signal */
};

/*!
 * \brief           SPI inital communication control bits on read
 */
enum Ltc68111SpiRicom {
    LTC6811_1_SPI_RICOM = 0b0111
};

/*!
 * \brief           SPI final communication control bits on read
 */
enum Ltc68111SpiRfcom {
    LTC6811_1_SPI_RFCOM = 0b1111
};

/*!
 * \brief           Configuration register group structure
 */
struct Ltc68111Cfgr {
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
struct Ltc68111Str {
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
struct Ltc68111Comm {
    uint8_t icom0 : 4;                     /*!< Initial communication control bits of the first data byte */
    uint8_t icom1 : 4;                     /*!< Initial communication control bits of the second data byte */
    uint8_t icom2 : 4;                     /*!< Initial communication control bits of the third data byte */
    uint8_t fcom0 : 4;                     /*!< Final communication control bits of the first data byte */
    uint8_t fcom1 : 4;                     /*!< Final communication control bits of the second data byte */
    uint8_t fcom2 : 4;                     /*!< Final communication control bits of the third data byte */
    uint8_t payload[LTC6811_1_COMM_COUNT]; /*!< Payload data transmited (received) to (from) I2C/SPI slave device */
} __attribute__((__packed__));

/*!
 * \brief           LTC6811-1 handler structure
 */
struct Ltc68111Handler {
    size_t count;
};

#endif
