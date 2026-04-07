#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "eagletrt-api.h"
#include "ltc6811-1-api.h"
#include "ltc6811-1.h"

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
     * The ADOW commands can be used to check for any open wires between the ADCs
     * of the LTC6811 and the external cells.
     * The algorithm used to check for open wire is the following:
     *     1. Run the 12-cell command ADOW with PUP=1 at least twice and read
     *        and store all cell voltages
     *     2. Run the 12-cell command ADOW with PUP=0 at least twice and read
     *        and store all cell voltages
     *     3. Take the difference between the pull-up and pull-down measurements
     *        in the above steps for cells from 2 to 12 (i.e. delta[i] = pup[i] - pud[i])
     *     4. For all values from 1 to 11:
     *         - If delta[i + 1] < -400mV then C(i) is open
     *         - If pup(1) = 0.0000 then C(0) is open
     *         - If pud(12) = 0.0000 then C(12) is open
     */

    /* Step 1. */
    for (size_t step = 0; step < 2; ++step) {
        /*
         * To start the conversion it is needed to encode and send the ADOW command`
         * providing the buffer of bytes to fill out and the necessary parameters,
         * in particular the 'PUP_ACTIVE' one that starts the conversion in Pull-up mode.
         *
         * The number of encoded bytes should be compared to the expected value
         * to check for possible errors while encoding the data.
         */
        uint8_t adow_pup[LTC6811_1_POLL_BUFFER_SIZE] = { 0 };
        const size_t adow_pup_byte_count = ltc6811_1_adow_encode_broadcast(
            &handler,
            LTC6811_1_MD_7KHZ,
            LTC6811_1_PUP_ACTIVE,
            LTC6811_1_DCP_DISABLED,
            LTC6811_1_CH_ALL,
            adow_pup);
        if (adow_pup_byte_count == LTC6811_1_POLL_BUFFER_SIZE) {
            send_payload_dummy(adow_pup, adow_pup_byte_count);
        } else {
            printf("[ERROR]: ADOW with Pull-up encode error on step %lu\n", step + 1);
        }
    }

    /*
     * To ensure that the conversion has ended it is needed to either:
     *     1. Wait enough time to be sure that it has ended
     *     2. Poll for the conversion status
     *
     * The first method does not require much explanation, the second tough
     * works as the ADOW command seen before. It is just needed to encode and
     * send the command with the appropriate checks.
     */
    uint8_t poll[LTC6811_1_POLL_BUFFER_SIZE] = { 0 };
    const size_t poll_byte_size = ltc6811_1_pladc_encode_broadcast(&handler, poll);
    if (poll_byte_size == LTC6811_1_POLL_BUFFER_SIZE) {
        send_payload_dummy(poll, poll_byte_size);
    } else {
        printf("[ERROR]: Poll encoding error");
    }

    /*
     * Once the command is issued to the IC the response can be compared using
     * the 'pladc_check' function to verify if the conversion has ended or not.
     *
     * In this example we already give the expected value once the conversion
     * has completed, in reality this value is given by the ICs themselfs.
     *
     * As a side note if using the SPI as communication protocol it should be
     * noted that driving the Chip Select high during the poll for conversion
     * status may cause false positives.
     */
    const uint8_t pladc_response = 0xff;
    if (ltc6811_1_pladc_is_completed(pladc_response)) {
        printf("[INFO]: Conversion has completed\n");
    } else {
        printf("[WARNING]: Conversion has not completed yet\n");
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
        const size_t read_byte_count = ltc6811_1_rdcv_encode_broadcast(
            &handler,
            reg,
            read);
        if (read_byte_count == LTC6811_1_READ_BUFFER_SIZE) {
            send_payload_dummy(read, read_byte_count);
        } else {
            printf("[ERROR]: Read encoding error for the register n°%u\n", reg);
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
        const size_t byte_count = ltc6811_1_rdcv_decode_broadcast(&handler, payload, voltages);
        if (byte_count == LTC6811_1_DATA_BUFFER_SIZE(LTC_COUNT)) {
            printf("[SUCCES]: Register n°%u correctly decoded\n", reg);
        } else {
            printf("[ERROR]: Read decoding error for the register n°%u\n", reg);
        }
    }

    /* Step 2. */
    /*
     * This step works in the same way as the first one with the only difference
     * that the ADOW command has to be called with 'PUP_INACTIVE'.
     * No code is provided since it is the almost same as the one above.
     */

    /* Step 3. */
    /*
     * The third step is to calculate the delta from the two previous measurements
     * In this example the values are all set to 0 but they should be read and
     * filled by the previous steps.
     */
    uint16_t pup[LTC_COUNT][LTC6811_1_CELL_COUNT] = { 0 };
    uint16_t pud[LTC_COUNT][LTC6811_1_CELL_COUNT] = { 0 };
    int16_t delta[LTC_COUNT][LTC6811_1_CELL_COUNT] = { 0 };
    for (size_t ltc = 0; ltc < LTC_COUNT; ++ltc) {
        for (size_t i = 1; i < LTC6811_1_CELL_COUNT; ++i) {
            delta[ltc][i] = pup[ltc][i] - pud[ltc][i];
        }
    }

    /* Step 4. */
    /*
     * The fourth step it is the actual check for any open wire as described
     * by the datasheet (and at the beginning of this example).
     * This step can be easily merged with step 3.
     */
    for (size_t ltc = 0; ltc < LTC_COUNT; ++ltc) {
        for (size_t i = 0; i < LTC6811_1_CELL_COUNT - 1; ++i) {
            if (delta[ltc][i + 1] < (-400 * 10)) {
                printf("[ERROR]: IC n°%lu, C(%lu) is open\n", ltc, i + 1);
            }
        }
        if (pup[ltc][0] == 0) {
            printf("[ERROR]: IC n°%lu, C(0) is open\n", ltc);
        }
        if (pud[ltc][LTC6811_1_CELL_COUNT - 1] == 0) {
            printf("[ERROR]: IC n°%lu, C(12) is open\n", ltc);
        }
    }

    return 0;
}
