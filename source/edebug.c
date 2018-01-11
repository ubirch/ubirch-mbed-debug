/*!
 * @file
 * @brief Implementations for the debug output functions.
 *
 * @author Matthias L. Jugel
 * @date   2017-11-04
 *
 * @copyright &copy; 2017 ubirch GmbH (https://ubirch.com)
 *
 * ```
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ```
 */

#if defined(TARGET_NORDIC)
#include <nrf52_bitfields.h>
#endif

#include <cmsis.h>
#include <stddef.h>
#include <string.h>
#include "edebug.h"


#if DEVICE_SERIAL && defined(EDEBUG_UART)
#include <serial_api.h>

static void *stdout_putp;
extern int stdio_uart_inited;
extern serial_t stdio_uart;

inline void __edebug_putc(void* putb, char c) {
    serial_putc(&stdio_uart, c);
}

void __edebug_setup(void *putp, void (*putf)(void *, char)) {
    if (!stdio_uart_inited) {
        serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
    }
    stdout_putp = putp;
    init_printf(putp, putf);
}

#elif defined(ENABLE_SWO) && defined(EDEBUG_SWO)

static void *stdout_putp;

inline void __edebug_putc(void *putb, char c) {
    // enable trace control and stimulus if needed
    if(!(ITM->TCR & 0x1) && !(ITM->TER & 0x1)) {
        ITM->TCR |= 1;
        ITM->TER |= 1;
    }

    ITM_SendChar((uint32_t) c);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
void __edebug_setup(void *putp, void (*putf)(void *, char)) {
#if defined(TARGET_NORDIC)
    NRF_CLOCK->TRACECONFIG = (NRF_CLOCK->TRACECONFIG & ~CLOCK_TRACECONFIG_TRACEPORTSPEED_Msk) |
                             (CLOCK_TRACECONFIG_TRACEPORTSPEED_4MHz << CLOCK_TRACECONFIG_TRACEPORTSPEED_Pos);
#elif defined(TARGET_KSDK2_MCUS)
    uint32_t SWOSpeed = 4000000; //6000kbps, default for JLinkSWOViewer
    uint32_t SWOPrescaler = (SystemCoreClock / SWOSpeed) - 1; // SWOSpeed in Hz, note that F_CPU is expected to be 96000000 in this case
    CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk;
    *((volatile unsigned *)(ITM_BASE + 0x400F0)) = 0x00000002; // "Selected PIN Protocol Register": Select which protocol to use for trace output (2: SWO)
    *((volatile unsigned *)(ITM_BASE + 0x40010)) = SWOPrescaler; // "Async Clock Prescaler Register". Scale the baud rate of the asynchronous output
    *((volatile unsigned *)(ITM_BASE + 0x00FB0)) = 0xC5ACCE55; // ITM Lock Access Register, C5ACCE55 enables more write access to Control Register 0xE00 :: 0xFFC
    ITM->TCR = ITM_TCR_TraceBusID_Msk | ITM_TCR_SWOENA_Msk | ITM_TCR_SYNCENA_Msk | ITM_TCR_ITMENA_Msk; // ITM Trace Control Register
    ITM->TPR = ITM_TPR_PRIVMASK_Msk; // ITM Trace Privilege Register
    ITM->TER = 0x00000001; // ITM Trace Enable Register. Enabled tracing on stimulus ports. One bit per stimulus port.
    *((volatile unsigned *)(ITM_BASE + 0x01000)) = 0x400003FE; // DWT_CTRL
    *((volatile unsigned *)(ITM_BASE + 0x40304)) = 0x00000100; // Formatter and Flush Control Register
#else
    //#error "no trace config for this platform"
#endif
    stdout_putp = putp;
    init_printf(putp, putf);
}
#pragma GCC diagnostic pop

#endif

void __edebug_hex_dump(const char *prefix, const uint8_t *b, size_t size) {
    for (unsigned int i = 0; i < size; i += 16) {
        if (prefix && strlen(prefix) > 0) __edebug_printf("%s %06x: ", prefix, i);
        for (unsigned int j = 0; j < 16; j++) {
            if ((i + j) < size) __edebug_printf("%02x", b[i + j]); else __edebug_printf("  ");
            if ((j + 1) % 2 == 0) __edebug_putc(stdout_putp, ' ');
        }
        __edebug_putc(stdout_putp, ' ');
        for (unsigned int j = 0; j < 16 && (i + j) < size; j++) {
            __edebug_putc(stdout_putp, (char) (b[i + j] >= 0x20 && b[i + j] <= 0x7E ? b[i + j] : '.'));
        }
        __edebug_printf("\r\n");
    }
}

void __edebug_hex_xxd(const char *prefix, const uint8_t *b, size_t size) {
    if (prefix && strlen(prefix) > 0) __edebug_printf("%s ", prefix);
    for (unsigned int i = 0; i < size; i++) __edebug_printf("%02x", b[i]);
    __edebug_printf("\r\n");
}
