set( TEST_NAME romz-os-utests )

add_executable( ${TEST_NAME}
    main.cpp
    File.cpp
)

add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )

target_compile_options( ${TEST_NAME} PRIVATE ${ROMZ_COMPILER_FLAGS} )

target_link_libraries( ${TEST_NAME} PRIVATE romz-os ${ROMZ_LINKER_FLAGS} )

