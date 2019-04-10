set( TEST_NAME romz-page_manager-utests )

add_executable( ${TEST_NAME}
    basic.cpp
    main.cpp
)

add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )

target_compile_options( ${TEST_NAME} PRIVATE ${ROMZ_COMPILER_FLAGS} )

target_link_libraries( ${TEST_NAME} PRIVATE romz-page_manager ${ROMZ_LINKER_FLAGS} )
