set( TEST_NAME romz-memory-utests )

add_executable( ${TEST_NAME}
    main.cpp
    Memory.cpp
)

add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )

target_compile_options( ${TEST_NAME} PRIVATE ${ROMZ_COMPILER_FLAGS} )

target_link_libraries( ${TEST_NAME} PRIVATE romz-memory ${ROMZ_LINKER_FLAGS} )

