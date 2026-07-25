#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include "eagletrt.h"
#include "eagletrt-api.h"
#include "ltc6810-2-api.h"
#include "ltc6810-2.h"

#include "stm32f4xx_hal.h" /*! Replace with your actual target header */

/* In this example only one IC is used for simplicity */
#define LTC_COUNT (1U)
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
    struct Ltc68102Handler handler;
    ltc6810_2_api_init(&handler, LTC_COUNT);

    /*
     * Then it is needed to prepare the configuration structures.
     * One is used to write the configuration, the other to read it from the IC.
     */
    struct Ltc68102Cfgr read_config = { 0 };
    struct Ltc68102Cfgr write_config = {
        .GPIO = 0b1111,
        .REFON = 1U,
    };

    /*
     * To write the configuration a buffer of bytes needs to be given to the
     * 'wrcfg_encode' function as well as the configuration structure.
     * Once executed the number of encoded bytes is returned and the buffer
     * is filled with the data to send to the IC.
     *
     * The number of encoded bytes should be compared to the expected value
     * to check for possible errors while encoding the data.
     */
    uint8_t write[LTC6810_2_WRITE_BUFFER_SIZE(LTC_COUNT)] = { 0 };
    const size_t write_byte_count = ltc6810_2_api_wrcfg_encode_broadcast(&handler, &write_config, write);
    if (write_byte_count == LTC6810_2_WRITE_BUFFER_SIZE(LTC_COUNT)) {
        HAL_SPI_Transmit(&hspi1, write, write_byte_count, 10U);
    } else {
        uart_printf("[ERROR]: Write encoding error\n");
    }

    /*
     * To read the configuration it is first needed to send the read command
     * to the IC.
     * This works the same as the write command except that it has no payload
     * to add to the command whatsoever.
     *
     * Same as before, the number of encoded bytes is compared to the expected
     * value to check for encoding errors.
     */
    uint8_t read[LTC6810_2_READ_BUFFER_SIZE] = { 0 };
    const size_t read_byte_count = ltc6810_2_api_rdcfg_encode_broadcast(&handler, read);
    if (read_byte_count == LTC6810_2_READ_BUFFER_SIZE) {
        HAL_SPI_Receive(&hspi1, read, read_byte_count, 10U);
    } else {
        uart_printf("[ERROR]: Read encoding error\n");
    }

    /*
     * Once the read command is dispatched the payload of bytes can be read from
     * the IC.
     * In this example a wrong payload is directly defined below, in reality it
     * should be read from the IC itself.
     *
     * Then the payload is decoded via the `rdcfg_decode` function that once
     * executed returns the number of decoded bytes and fills out the
     * configuration structure.
     *
     * Since in this example the payload data is wrong the configuration structure
     * is not modified and the program should print an error indicating that
     * the decoded data is invalid.
     */
    uint8_t payload[LTC6810_2_DATA_BUFFER_SIZE(LTC_COUNT)] = { 1, 2, 3, 4, 5 };
    const size_t byte_count = ltc6810_2_api_rdcfg_decode_broadcast(&handler, payload, &read_config);
    if (byte_count == LTC6810_2_DATA_BUFFER_SIZE(LTC_COUNT)) {
        uart_printf("[SUCCESS]: Configuration correctly read\n");
    } else {
        uart_printf("[ERROR]: Read decoding error\n");
    }

    return 0;
}
