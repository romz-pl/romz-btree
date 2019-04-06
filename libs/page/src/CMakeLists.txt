set( LIB_NAME romz-page )

add_library( ${LIB_NAME} STATIC
    Page.cpp
    PPageData.cpp
    PPageHeader.cpp
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
    romz-container
    romz-memory
    PRIVATE
    # here add private libraries
)

