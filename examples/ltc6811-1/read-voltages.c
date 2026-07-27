#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include "eagletrt.h"
#include "eagletrt-api.h"
#include "ltc6811-1-api.h"
#include "ltc6811-1.h"

#include "stm32f4xx_hal.h" /*! Replace with your actual target header */

#define LTC_COUNT (2U)
#define UART_BUFFER_SIZE (1024U)

UART_HandleTypeDef huart1;
SPI_HandleTypeDef hspi1;

/*! Function definitions to suppress "is not implemented and will always fail" warning */
void _close(void) {
}

void _lseek(void) {
}

void _read(void) {
}

void _write(void) {
}

/*!
 * \brief           HAL_UART_Transmit wrapper with a printf-like prototype.
 *
 * \param[in]       fmt A pointer to the format string.
 * \param[in]       ... Variadic arguments.
 */
void uart_printf(const char *fmt, ...) {
    static uint8_t buffer[UART_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);

    int len = vsnprintf((char *)buffer, UART_BUFFER_SIZE, fmt, args);
    va_end(args);

    if (len > 0) {
        HAL_UART_Transmit(&huart1, buffer, (uint16_t)len, HAL_MAX_DELAY);
    }
}

int main(void) {
    /* The first thing needed is to declare and initialize the handler structure */
    struct Ltc68111Handler handler;
    ltc6811_1_api_init(&handler, LTC_COUNT);

    /*
     * To read the voltages it is first needed to start the ADC conversion by
     * sending the ADCV command to all the ICs with the appropriate parameters.
     * For this a buffer of bytes is needed which is filled by the 'adcv_encode'
     * function that returns the number of encoded bytes.
     *
     * In this example the conversion is started on all ADC channels at the
     * same time.
     *
     * The number of encoded bytes should be compared to the expected value
     * to check for possible errors while encoding the data.
     */
    uint8_t start_conversion_payload[LTC6811_1_POLL_BUFFER_SIZE] = { 0 };
    const size_t adcv_byte_size = ltc6811_1_api_adcv_encode_broadcast(
        &handler,
        LTC6811_1_MD_27KHZ,
        LTC6811_1_DCP_DISABLED,
        LTC6811_1_CH_ALL,
        start_conversion_payload);
    if (adcv_byte_size == LTC6811_1_POLL_BUFFER_SIZE) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_SPI_Transmit(&hspi1, start_conversion_payload, adcv_byte_size, 10U);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    } else {
        uart_printf("[ERROR]: Start conversion encoding error");
    }

    /*
     * To ensure that the conversion has ended it is needed to either:
     *     1. Wait enough time to be sure that it has ended
     *     2. Poll for the conversion status
     *
     * The first method does not require much explanation, the second tough
     * works as the ADCV command seen before. It is just needed to encode and
     * send the command with the appropriate checks.
     */
    uint8_t poll[LTC6811_1_POLL_BUFFER_SIZE] = { 0 };
    const size_t poll_byte_size = ltc6811_1_api_pladc_encode_broadcast(&handler, poll);
    if (poll_byte_size == LTC6811_1_POLL_BUFFER_SIZE) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_SPI_Transmit(&hspi1, poll, poll_byte_size, 10U);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    } else {
        uart_printf("[ERROR]: Poll encoding error");
    }

    /*
     * Once the command is issued to the IC the response can be compared using
     * the 'pladc_is_completed' function to verify if the conversion has ended or not.
     *
     * In this example we already give the expected value once the conversion
     * has completed, in reality this value is given by the ICs themselfs.
     *
     * As a side note if using the SPI as communication protocol it should be
     * noted that driving the Chip Select high during the poll for conversion
     * status may cause false positives.
     */
    const uint8_t pladc_response = 0xff;
    if (ltc6811_1_api_pladc_is_completed(pladc_response)) {
        uart_printf("[INFO]: Conversion has completed\n");
    } else {
        uart_printf("[WARNING]: Conversion has not completed yet\n");
    }

    /*
     * After the conversion has completed it can be possible to read all the
     * voltages by issuing multiple read commands, one for each register.
     * Only one single register at a time can be read even if all the ICs sends
     * their data in the same message.
     *
     * The ICs may not like receiving multiple commands in a short amount of
     * time, especially if they are already doing other things. It is left to
     * the programmer to ensure that enough time is given to the IC to ensure
     * the correctness of the operations.
     */
    for (enum Ltc68111Cvxr reg = 0; reg < LTC6811_1_CVXR_COUNT; ++reg) {
        /*
         * Same as before, the command to read the voltages is encoded and sent
         * with the register as additional parameter.
         */
        uint8_t read[LTC6811_1_READ_BUFFER_SIZE] = { 0 };
        const size_t read_byte_count = ltc6811_1_api_rdcv_encode_broadcast(
            &handler,
            reg,
            read);
        if (read_byte_count == LTC6811_1_READ_BUFFER_SIZE) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
            HAL_SPI_Receive(&hspi1, read, read_byte_count, 10U);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
        } else {
            uart_printf("[ERROR]: Read encoding error for the register n°%u\n", reg);
        }

        /*
         * After issuing the read command the voltages can be received and decoded.
         * In this example a wrong payload is directly defined below, in reality it
         * should be read from the IC itself.
         *
         * Since in this example the payload data is wrong the voltages array
         * is not modified and the program should print an error indicating that
         * the decoded data is invalid.
         */
        uint8_t payload[LTC6811_1_DATA_BUFFER_SIZE(LTC_COUNT)] = { 1, 2, 3, 4, 5 };
        uint16_t voltages[LTC6811_1_REG_CELL_COUNT * LTC_COUNT] = { 0 };
        const size_t byte_count = ltc6811_1_api_rdcv_decode_broadcast(&handler, payload, voltages);
        if (byte_count == LTC6811_1_DATA_BUFFER_SIZE(LTC_COUNT)) {
            uart_printf("[SUCCES]: Register n°%u correctly decoded\n", reg);
        } else {
            uart_printf("[ERROR]: Read decoding error for the register n°%u\n", reg);
        }
    }
    return 0;
}
