/*!
 * @file
 * @brief TODO: ${FILE}
 *
 * ...
 *
 * @author Matthias L. Jugel
 * @date   2017-11-06
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

#include "edebug.h"

#if MBED_CONF_RTOS_PRESENT

#include "rtos/rtos.h"

#if osRtxVersionAPI
void __edebug_mbed_print_threads() {
    core_util_critical_section_enter();
    for (struct osRtxThread_s *t = osRtxInfo.thread.delay_list; t != NULL; t = t->thread_next) {
        EDEBUG_PRINTF("DELY: 0x%08x: 0x%08x [%s] %d\r\n", t, t->thread_addr, t->name, t->stack_size);
    }
    for (struct osRtxThread_s *t = osRtxInfo.thread.wait_list; t != NULL; t = t->thread_next) {
        EDEBUG_PRINTF("WAIT: 0x%08x: 0x%08x [%s] %d\r\n", t, t->thread_addr, t->name, t->stack_size);
    }
    for (struct osRtxThread_s *t = osRtxInfo.thread.idle; t != NULL; t = t->thread_next) {
        EDEBUG_PRINTF("IDLE: 0x%08x: 0x%08x [%s] %d\r\n", t, t->thread_addr, t->name, t->stack_size);
    }
    core_util_critical_section_exit();
}
#else
void __edebug_mbed_print_threads() {}
#endif

#endif
