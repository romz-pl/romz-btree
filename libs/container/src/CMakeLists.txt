set( LIB_NAME romz-container )

add_library( ${LIB_NAME} STATIC
    ArrayView.cpp
    ByteArrayView.cpp
    DynamicArray.cpp
)

target_compile_options( ${LIB_NAME} PRIVATE ${ROMZ_COMPILER_FLAGS} )

target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(
    ${LIB_NAME}
    PUBLIC
    romz-memory
    PRIVATE
    # here add private libraries
)

