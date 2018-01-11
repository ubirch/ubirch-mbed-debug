add_executable(debug-tests
        TESTS/edebug/basic/DebugTests.cpp
        )
target_link_libraries(debug-tests mbed-os edebug)

add_custom_target(compile-tests ALL
        COMMAND mbed test --compile -n tests-edebug* -vv --profile mbed-os/tools/profiles/debug.json --app-config TESTS/settings.json
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_target(run-tests ALL
        COMMAND mbed test -n tests-edebug* -vv --profile mbed-os/tools/profiles/debug.json --app-config TESTS/settings.json
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})