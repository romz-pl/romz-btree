set( LIB_NAME romz-device )

add_library( ${LIB_NAME} STATIC
    Device.cpp
    DiskDevice.cpp
    DiskState.cpp
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
    romz-config
    romz-page
    romz-os
    PRIVATE
    # here add private libraries
)

