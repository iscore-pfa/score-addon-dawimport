project(score_addon_dawimport LANGUAGES CXX)

# General initialization
score_common_setup()

# Source files
set(HDRS
    DawImport/ApplicationPlugin.hpp

    score_addon_dawimport.hpp
)
set(SRCS
    DawImport/ApplicationPlugin.cpp

    score_addon_dawimport.cpp
)


# Creation of the library
add_library(${PROJECT_NAME} ${SRCS} ${HDRS})

# Code generation
score_generate_command_list_file(${PROJECT_NAME} "${HDRS}")

# Link
target_link_libraries(${PROJECT_NAME} PUBLIC score_plugin_engine score_plugin_midi score_plugin_media)
target_compile_definitions(${PROJECT_NAME} PRIVATE PLUGIN_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")

# Target-specific options
setup_score_plugin(${PROJECT_NAME})
