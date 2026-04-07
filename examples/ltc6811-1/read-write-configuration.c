#include <stdint.h>
#include <stdio.h>

#include "eagletrt-api.h"
#include "ltc6811-1-api.h"
#include "ltc6811-1.h"

/* In this example only one IC is used for simplicity */
#define LTC_COUNT (1U)

/*
 * This function does not actually send the payload since the transmission
 * does depend on the type of hardware used to communicate with the IC
 */
void send_payload_dummy(uint8_t *payload, const size_t len) {
    EAGLETRT_API_UNUSED(payload);
    printf("[INFO]: Sending %lu bytes of payload\n", len);
}

int main(void) {
    /* The first thing needed is to declare and initialize the handler structure */
    struct Ltc68111Handler handler;
    ltc6811_1_init(&handler, LTC_COUNT);

    /*
     * Then it is needed to prepare the configuration structures.
     * One is used to write the configuration, the other to read it from the IC.
     */
    struct Ltc68111Cfgr read_config = { 0 };
    struct Ltc68111Cfgr write_config = {
        .GPIO = 0b11111,
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
    uint8_t write[LTC6811_1_WRITE_BUFFER_SIZE(LTC_COUNT)] = { 0 };
    const size_t write_byte_count = ltc6811_1_wrcfg_encode_broadcast(&handler, &write_config, write);
    if (write_byte_count == LTC6811_1_WRITE_BUFFER_SIZE(LTC_COUNT)) {
        send_payload_dummy(write, write_byte_count);
    } else {
        printf("[ERROR]: Write encoding error\n");
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
    uint8_t read[LTC6811_1_READ_BUFFER_SIZE] = { 0 };
    const size_t read_byte_count = ltc6811_1_rdcfg_encode_broadcast(&handler, read);
    if (read_byte_count == LTC6811_1_READ_BUFFER_SIZE) {
        send_payload_dummy(read, read_byte_count);
    } else {
        printf("[ERROR]: Read encoding error\n");
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
    uint8_t payload[LTC6811_1_DATA_BUFFER_SIZE(LTC_COUNT)] = { 1, 2, 3, 4, 5 };
    const size_t byte_count = ltc6811_1_rdcfg_decode_broadcast(&handler, payload, &read_config);
    if (byte_count == LTC6811_1_DATA_BUFFER_SIZE(LTC_COUNT)) {
        printf("[SUCCESS]: Configuration correctly read\n");
    } else {
        printf("[ERROR]: Read decoding error\n");
    }

    return 0;
}
