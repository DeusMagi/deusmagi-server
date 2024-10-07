cmake_minimum_required(VERSION 3.12)

project(deusmagi-server C)

set(EXECUTABLE deusmagi-server)
set(PACKAGE_NAME "Deus Magi Server")

find_package(FLEX REQUIRED)
find_library(DL_LIBRARY dl)

include_directories(common)
include_directories(src/http_server)
include_directories(src/include)
include_directories(src/random_maps)
include_directories(src/tests)
include_directories(src/types/monster/include)
include_directories(src/types/include)
include_directories(src)

add_definitions(-Wall -Wextra -Wno-unused-parameter -Wno-deprecated-declarations -Wno-format-truncation -Wno-format-overflow -Wno-implicit-fallthrough -Wno-int-conversion -Wno-cast-function-type -Wno-discarded-qualifiers -Wno-implicit-function-declaration -Wno-missing-field-initializers -Wno-unused-function -D_GNU_SOURCE -D__USE_MINGW_ANSI_STDIO=0 -std=gnu99 -ggdb -O0)

set(LINUX true)
set(PLUGIN_SUFFIX ".so")

set(LEXERS
    src/loaders/map_header.l
    src/loaders/object.l
    src/loaders/random_map.l)
set(LEXERS_OUT)

# parse the lexer files with flex
foreach (name ${LEXERS})
    string(REPLACE ".l" "" name ${name})
    set(src "${name}.l")
    set(dst "${name}.c")
    string(REPLACE "src/loaders/" "" name ${name})
    flex_target(${name} "${PROJECT_SOURCE_DIR}/${src}" "${PROJECT_SOURCE_DIR}/${dst}" COMPILE_FLAGS "-Cfa -i --prefix yy_${name}")
    get_property(out VARIABLE PROPERTY FLEX_${name}_OUTPUTS)
    set(LEXERS_OUT ${LEXERS_OUT} ${out})
endforeach (name)

include(src/cmake.txt)

find_library(GD_LIBRARY gd)

if (GD_LIBRARY)
    set(HAVE_WORLD_MAKER true)
else ()
    message(STATUS "Could not find gd library, world maker module will be disabled.")
endif ()

if (HAVE_WORLD_MAKER)
    set(SOURCES_WORLD_MAKER
        src/modules/world_maker.c)
endif ()

add_executable(${EXECUTABLE} ${SOURCES} ${LEXERS_OUT} ${SOURCES_CHECK} ${SOURCES_WORLD_MAKER})

target_link_libraries(${EXECUTABLE} deusmagi-toolkit)
target_link_libraries(${EXECUTABLE} -Xlinker --allow-multiple-definition)
target_link_libraries(${EXECUTABLE} ${DL_LIBRARY})

if (GD_LIBRARY)
    target_link_libraries(${EXECUTABLE} ${GD_LIBRARY})
endif ()

get_directory_property(INCLUDES INCLUDE_DIRECTORIES)

set(SOURCES_PLUGIN_ARENA src/plugins/plugin_arena/plugin_arena.c)
add_library(plugin_arena MODULE ${SOURCES_PLUGIN_ARENA})

set(Python_ADDITIONAL_VERSIONS 3.10)
set(CMAKE_USE_PYTHON_VERSION ${Python_ADDITIONAL_VERSIONS})
find_package(PythonLibs)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(src/plugins/plugin_python/include)
set(HAVE_PYTHON true)

set(SOURCES_PLUGIN_PYTHON
    src/plugins/plugin_python/atrinik_archetype.c
    src/plugins/plugin_python/atrinik_map.c
    src/plugins/plugin_python/atrinik_object.c
    src/plugins/plugin_python/atrinik_party.c
    src/plugins/plugin_python/atrinik_player.c
    src/plugins/plugin_python/atrinik_region.c
    src/plugins/plugin_python/attr_list.c
    src/plugins/plugin_python/plugin_python.c
)

add_library(plugin_python MODULE ${SOURCES_PLUGIN_PYTHON})
target_link_libraries(plugin_python ${PYTHON_LIBRARIES})
target_link_libraries(plugin_python -Xlinker --allow-multiple-definition)

configure_file(src/include/cmake.h.def src/include/cmake.h)
configure_file(src/include/version.h.def src/include/version.h)

add_subdirectory(common/toolkit)