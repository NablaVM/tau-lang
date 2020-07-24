
link_directories(${CPPUTEST_LIBRARIES})

include_directories(${PARSER_DIR})

add_executable(tau_parser_test
        TokenStreamTest.cpp
        main.cpp
)

message(${CPPUTEST_LDFLAGS})

target_link_libraries(tau_parser_test 
                      tau-lib
                      ${CPPUTEST_LDFLAGS})

add_custom_command(TARGET tau_parser_test COMMAND ./tau_parser_test POST_BUILD)
