file(GLOB DOOM_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../../../linuxdoom-1.10/*.c")

list(REMOVE_ITEM DOOM_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/../../../linuxdoom-1.10/i_main.c"
    "${CMAKE_CURRENT_LIST_DIR}/../../../linuxdoom-1.10/i_net.c"
    "${CMAKE_CURRENT_LIST_DIR}/../../../linuxdoom-1.10/i_sound.c"
    "${CMAKE_CURRENT_LIST_DIR}/../../../linuxdoom-1.10/i_video.c"
)

idf_component_register(
    SRCS ${DOOM_SOURCES}
    INCLUDE_DIRS "../../../linuxdoom-1.10"
)
