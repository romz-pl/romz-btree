set( LIB_NAME romz-os )

add_library( ${LIB_NAME} STATIC
    enable_largefile.cpp
    exception_with_errno.cpp
    File.cpp
    lock_exclusive.cpp
    os_read.cpp
    os_write.cpp
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
    PRIVATE
    # here add private libraries
)

