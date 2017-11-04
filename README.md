# mbed debug output functions

Using printf for debugging can be quite a memory and flash hog. If you want to get rid
of it, use this little library. Additionally, it can send its output also via the SWO
line of your debugger.

```bash
mbed add https://github.com/ubirch/ubirch-mbed-debug
```

## Usage

- compile your code with `-DENABLE_SWO -DEDEBUG_SWO` to enable SWO
- in `main()` or some other convenient place, call `EDEBUG_SETUP(NULL)`
- use the `EDEBUG_*` macros:
    - `EDEBUG_PRINTF(fmt, ...)` - printf to debug out
    - `EDEBUG_DUMP(prefix, buffer, size)` - dump a buffer in a nicely formatted way
    - `EDEBUG_HEX(prefix, buffer, size)` - dump a buffer simply as hex
    - `EDEBUG_SPRINTF(buffer, fmt, ...)` - write sprintf to buffer

> If neither `ENABLE_SWO` and `EDEBUG_SWO` or `DEVICE_SERIAL` and `EDEBUG_UART` are defined,
> the macros will do nothing.

In mbed you can enable SWO debug in a convenient way in `app_config.json`:

```json
{
  "target_overrides": {
    "*": {
      "target.macros_add": ["ENABLE_SWO","EDEBUG_SWO"]
    }
  }
}
```

## Testing

```bash
mbed new .
mbed target NRF52_DK
mbed toolchain GCC_ARM
mbed test -n tests-debug* -vv
```

### Results

SWO output:

```
+------------------+---------------+--------------------+--------+--------------------+-------------+
| target           | platform_name | test suite         | result | elapsed_time (sec) | copy_method |
+------------------+---------------+--------------------+--------+--------------------+-------------+
| NRF52_DK-GCC_ARM | NRF52_DK      | tests-edebug-basic | OK     | 20.77              | default     |
+------------------+---------------+--------------------+--------+--------------------+-------------+
mbedgt: test suite results: 1 OK
mbedgt: test case report:
+------------------+---------------+--------------------+-------------------------+--------+--------+--------+--------------------+
| target           | platform_name | test suite         | test case               | passed | failed | result | elapsed_time (sec) |
+------------------+---------------+--------------------+-------------------------+--------+--------+--------+--------------------+
| NRF52_DK-GCC_ARM | NRF52_DK      | tests-edebug-basic | Test debug hex dump     | 1      | 0      | OK     | 1.04               |
| NRF52_DK-GCC_ARM | NRF52_DK      | tests-edebug-basic | Test debug output       | 1      | 0      | OK     | 0.5                |
| NRF52_DK-GCC_ARM | NRF52_DK      | tests-edebug-basic | Test debug xxd hex dump | 1      | 0      | OK     | 0.9                |
+------------------+---------------+--------------------+-------------------------+--------+--------+--------+--------------------+
```
# License

Author: Matthias L. Jugel ([@thinkberg](https://github.com/thinkberg))

This library is available under the [Apache License](LICENSE)

```
Copyright 2017 ubirch GmbH

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
````

Portions (printf) are licensed under the BSD License:

```
Copyright (c) 2004,2012 Kustaa Nyholm / SpareTimeLabs
All rights reserved.
```