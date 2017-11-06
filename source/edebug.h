/*!
 * @file
 * @brief Debug function and macros definitions.
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
#ifndef UBIRCH_MBED_BLE_MIN_DEBUG_H
#define UBIRCH_MBED_BLE_MIN_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(EDEBUG_SWO) && defined(EDEBUG_UART)

// you need to select one of the ways to debug
#error "select either EDEBUG_SWO or EDEBUG_UART"

#elif defined(EDEBUG_SWO) || defined(EDEBUG_UART)

#include <stdint.h>
#include <stddef.h>
#include "printf/printf.h"
#undef printf
#undef sprintf

/**
 * Setup the debug library.
 * @param putp a pointer to storage space if needed by the putc() function, normally NULL
 * @param putf a pointer to the putc() function (UART, SWO, etc.)
 */
void __edebug_setup(void *putp, void (*putf)(void *, char));

/**
 * Print a character to the debug output. This is either UART or SWO
 * @param p a buffer you could use (if not directly printing)
 * @param c the character to print
 */
void __edebug_putc(void *p, char c);

/**
 * Dump a buffer in a formatted way with ascii conversion where possible.
 * @param prefix a prefix to prepend to lines
 * @param b the buffer to dump
 * @param size the size of the buffer
 */
void __edebug_hex_dump(const char *prefix, const uint8_t *b, size_t size);

/**
 * Dump a buffer in a simple fashion as hex, no spaces, no formatting. Can be converted
 * back to binary using the xxd -r -ps command.
 * @param prefix a prefix to prepend to the line
 * @param b the buffer to dump
 * @param size the size of the buffer
 */
void __edebug_hex_xxd(const char *prefix, const uint8_t *b, size_t size);

#if MBED_CONF_RTOS_PRESENT

/**
 * Print the threads that are waiting, delayed or running in the RTOS.
 */
void __edebug_mbed_print_threads();

#define EDEBUG_PRINT_THREADS()  __edebug_mbed_print_threads()           //!< print threads in the system

#endif

// lets wrap the printf/sprintf
#define __edebug_printf(...)    tfp_printf(__VA_ARGS__)
#define __edebug_sprintf(...)   tfp_sprintf(__VA_ARGS__)

#define EDEBUG_SETUP(ptr)       __edebug_setup((ptr), __edebug_putc)    //!< setup edebug
#define EDEBUG_DUMP(p, b, l)    __edebug_hex_dump(p, b, l)              //!< hex dump a buffer formatted
#define EDEBUG_HEX(p, b, l)     __edebug_hex_xxd(p, b, l)               //!< hex dump a buffer in simple form
#define EDEBUG_PRINTF(...)      __edebug_printf(__VA_ARGS__)            //!< print a string
#define EDEBUG_SPRINTF(...)     __edebug_sprintf(__VA_ARGS__)           //!< print string to buffer


#else

#define EDEBUG_PRINT_THREADS()  ((void)0) //!< do not print thread info

#define __edebug_putc(...)      ((void)0) //!< putc disabled
#define __edebug_printf(...)    ((void)0) //!< printf disabled
#define __edebug_sprintf(...)   ((void)0) //!< sprintf disabled
#define EDEBUG_SETUP(ptr)       ((void)0) //!< no setup edebug
#define EDEBUG_DUMP(p, b, l)    ((void)0) //!< no hex dump a buffer
#define EDEBUG_HEX(p, b, l)     ((void)0) //!< no hex dump simple
#define EDEBUG_PRINTF(...)      ((void)0) //!< no print a string
#define EDEBUG_SPRINTF(...)     ((void)0) //!< no print string to buffer

#endif
#ifdef __cplusplus
};
#endif

#endif //UBIRCH_MBED_BLE_MIN_DEBUG_H
