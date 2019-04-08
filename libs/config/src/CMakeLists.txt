set( LIB_NAME romz-config )

add_library( ${LIB_NAME} STATIC
    DbConfig.cpp
    EnvConfig.cpp
    key_types.cpp
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
    # here add public libraries
    PRIVATE
    # here add private libraries
)

