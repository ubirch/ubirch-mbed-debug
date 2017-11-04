/*!
 * @file
 * @brief Test for simple debug functions.
 *
 * @author Matthias L. Jugel
 * @date   2017-10-02
 *
 * @copyright &copy; 2017 ubirch GmbH (https://ubirch.com)
 *
 * @section LICENSE
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

#include <edebug.h>
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

using namespace utest::v1;

void TestDebugOutput() {
    char k[20], v[20];
    greentea_send_kv("expect", "(HELLO WORLD!)");

    greentea_parse_kv(k, v, sizeof(k), sizeof(v));
    TEST_ASSERT_EQUAL_STRING("swo-ready", k);

    EDEBUG_SETUP(NULL);
    EDEBUG_PRINTF("HELLO WORLD!");

    greentea_parse_kv(k, v, sizeof(k), sizeof(v));
    TEST_ASSERT_EQUAL_STRING("received", k);
    TEST_ASSERT_EQUAL_STRING("HELLO WORLD!", v);
}

void TestDebugHexDump() {
    char k[20], v[20];
    greentea_send_kv("expect", "(PREFIX) (000000): (4845 4c4c 4f) +(HELLO)");

    greentea_parse_kv(k, v, sizeof(k), sizeof(v));
    TEST_ASSERT_EQUAL_STRING("swo-ready", k);

    const char *testBuffer = "HELLO";

    EDEBUG_SETUP(NULL);
    EDEBUG_DUMP("PREFIX", (const uint8_t *) testBuffer, strlen(testBuffer));

    greentea_parse_kv(k, v, sizeof(k), sizeof(v));
    TEST_ASSERT_EQUAL_STRING("received", k);
    TEST_ASSERT_EQUAL_STRING("PREFIX", v);

    greentea_parse_kv(k, v, sizeof(k), sizeof(v));
    TEST_ASSERT_EQUAL_STRING("received", k);
    TEST_ASSERT_EQUAL_STRING("000000", v);

    greentea_parse_kv(k, v, sizeof(k), sizeof(v));
    TEST_ASSERT_EQUAL_STRING("received", k);
    TEST_ASSERT_EQUAL_STRING("4845 4c4c 4f", v);

    greentea_parse_kv(k, v, sizeof(k), sizeof(v));
    TEST_ASSERT_EQUAL_STRING("received", k);
    TEST_ASSERT_EQUAL_STRING("HELLO", v);
}

void TestDebugXxdDump() {
    char k[20], v[20];
    greentea_send_kv("expect", "(PREFIX) (48454c4c4f)");

    greentea_parse_kv(k, v, sizeof(k), sizeof(v));
    TEST_ASSERT_EQUAL_STRING("swo-ready", k);

    const char *testBuffer = "HELLO";

    EDEBUG_SETUP(NULL);
    EDEBUG_HEX("PREFIX", (const uint8_t *) testBuffer, strlen(testBuffer));

    greentea_parse_kv(k, v, sizeof(k), sizeof(v));
    TEST_ASSERT_EQUAL_STRING("received", k);
    TEST_ASSERT_EQUAL_STRING("PREFIX", v);

    greentea_parse_kv(k, v, sizeof(k), sizeof(v));
    TEST_ASSERT_EQUAL_STRING("received", k);
    TEST_ASSERT_EQUAL_STRING("48454c4c4f", v);
}


utest::v1::status_t case_teardown_handler(const Case *const source, const size_t passed, const size_t failed,
                                          const failure_t reason) {
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "DebugTests");
    return verbose_test_setup_handler(number_of_cases);
}

int main() {
    Case cases[] = {
            Case("Test debug output", TestDebugOutput,
                 case_teardown_handler, greentea_failure_handler),
            Case("Test debug hex dump", TestDebugHexDump,
                 case_teardown_handler, greentea_failure_handler),
            Case("Test debug xxd hex dump", TestDebugXxdDump,
                 case_teardown_handler, greentea_failure_handler),
    };

    Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);
    return !Harness::run(specification);
}