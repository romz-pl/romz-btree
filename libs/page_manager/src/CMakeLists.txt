set( LIB_NAME romz-page_manager )

add_library( ${LIB_NAME} STATIC
    Freelist.cpp
    Pickle.cpp
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
    romz-page
    PRIVATE
    # here add private libraries
)

