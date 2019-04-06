set( TEST_NAME romz-container-utests )

add_executable( ${TEST_NAME}
    ArrayView.cpp
    DynamicArray.cpp
    main.cpp
)

add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )

target_compile_options( ${TEST_NAME} PRIVATE ${ROMZ_COMPILER_FLAGS} )

target_link_libraries( ${TEST_NAME} PRIVATE romz-container ${ROMZ_LINKER_FLAGS} )

