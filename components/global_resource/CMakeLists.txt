file(GLOB SOURCES
        *.c
        )

idf_component_register(
        SRCS ${SOURCES}
        INCLUDE_DIRS "."
        REQUIRES utils wt_common
)