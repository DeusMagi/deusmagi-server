project(deusmagi-server C)
cmake_minimum_required(VERSION 3.2)
set(EXECUTABLE deusmagi-server)
set(PACKAGE_NAME "Deus Magi Server")

# Need Flex...
find_package(FLEX REQUIRED)

# libdl is required.
find_library(DL_LIBRARY dl)

if (NOT DL_LIBRARY AND NOT MINGW)
    message(FATAL_ERROR "Could not find dl library.")
endif ()

# Add our includes.
include_directories(common)
include_directories(src/http_server)
include_directories(src/include)
include_directories(src/random_maps)
include_directories(src/tests)
include_directories(src/types/monster/include)
include_directories(src/types/include)
include_directories(src)

# Add some useful compile flags.
add_definitions(-Wall -Wextra -Wno-unused-parameter -Wno-deprecated-declarations -Wno-format-truncation -Wno-format-overflow -Wno-implicit-fallthrough -Wno-int-conversion -Wno-cast-function-type -Wno-discarded-qualifiers -Wno-implicit-function-declaration -Wno-missing-field-initializers -Wno-unused-function -D_GNU_SOURCE -D__USE_MINGW_ANSI_STDIO=0 -std=gnu99 -ggdb -O0)
add_definitions(${CUSTOM_WARNINGS})

if (NOT MINGW)
    if (ENABLE_STACK_PROTECTOR)
        add_definitions(-fstack-protector-all -Wstack-protector)
    else ()
        add_definitions(-fno-stack-protector)
    endif ()
endif ()

if (ENABLE_EXTRA_WARNINGS)
    # Non-macro identifiers inside #if outside of defined()
    add_definitions(-Wundef)
    # sizeof() on function types or void
    add_definitions(-Wpointer-arith)
    # Casting function types to non-matching types
    add_definitions(-Wbad-function-cast)
    # extern declaration within a function
    add_definitions(-Wnested-externs)
    # Dead code (removed in newer versions of GCC, might still be in clang)
    add_definitions(-Wunreachable-code)
    # Additional format checks for printf/scanf/etc not enabled by -Wall
    add_definitions(-Wformat=2 -Wno-format-nonliteral)
    # Declarations like int i = i
    add_definitions(-Winit-self)
    # Local variables or types that shadow another variable, parameter, type,
    # built-in function, etc.
    add_definitions(-Wshadow)
    # Global function defined without a previous declaration.
    add_definitions(-Wmissing-declarations)
    # Variable that may be clobbered by a setjmp call
    add_definitions(-Wuninitialized)
    # Testing floating point values for equality
    add_definitions(-Wfloat-equal)

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # Suspicious uses of logical operators in expressions
        add_definitions(-Wlogical-op)
        # Pointer casting with increased alignment (char * -> int *)
        add_definitions(-Wcast-align)
    endif ()
endif ()

if (ENABLE_AGGRESSIVE_WARNINGS)
    # Casting const char * to char *
    add_definitions(-Wcast-qual)
    # Assigning signed int to unsigned int
    add_definitions(-Wsign-conversion)
    # Loop indices that could potentially overflow
    add_definitions(-Wunsafe-loop-optimizations)
    # Redundant (multiple same) declarations
    add_definitions(-Wredundant-decls)
endif ()

# Turn warnings into errors?
if (ENABLE_WARNING_ERRORS)
    add_definitions(-Werror)
endif ()

if (MINGW)
    add_definitions(-DCURL_STATICLIB -DPTW32_STATIC_LIB -DBGDWIN32 -DNONDLL)
    add_definitions(-DNOCRYPT)
    set(CURL_STATICLIB true)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
    set(PLUGIN_SUFFIX ".dll")
    set(CMAKE_PREFIX_PATH "C:\\MinGW\\msys\\1.0;C:\\MinGW")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so;.dll.a")
else ()
    # Compiling on GNU/Linux.
    set(LINUX true)
    set(PLUGIN_SUFFIX ".so")
endif ()

# Scan for lexers.
set(LEXERS
    src/loaders/map_header.l
    src/loaders/object.l
    src/loaders/random_map.l)
set(LEXERS_OUT)

# Add command to parse the lexer files with flex.
foreach (name ${LEXERS})
    string(REPLACE ".l" "" name ${name})
    set(src "${name}.l")
    set(dst "${name}.c")
    string(REPLACE "src/loaders/" "" name ${name})
    flex_target(${name} "${PROJECT_SOURCE_DIR}/${src}" "${PROJECT_SOURCE_DIR}/${dst}" COMPILE_FLAGS "-Cfa -i --prefix yy_${name}")
    get_property(out VARIABLE PROPERTY FLEX_${name}_OUTPUTS)
    set(LEXERS_OUT ${LEXERS_OUT} ${out})
endforeach (name)

# Add sources.
include(src/cmake.txt)

# Check is used for unit tests, but is optional.
find_library(CHECK_LIBRARY check)

if (CHECK_LIBRARY)
    set(HAVE_CHECK true)
else ()
    message(STATUS "Could not find check library, unit tests will be disabled.")
endif ()

# GD is used for world maker, but like unit tests, is also optional.
find_library(GD_LIBRARY gd)

if (GD_LIBRARY)
    set(HAVE_WORLD_MAKER true)
else ()
    message(STATUS "Could not find gd library, world maker module will be disabled.")
endif ()

# Find unit test source files if check is installed.
if (CHECK_LIBRARY)
    set(SOURCES_CHECK
        src/tests/check.c
        src/tests/bugs/cursed_treasures.c
        src/tests/unit/commands/object.c
        src/tests/unit/server/arch.c
        src/tests/unit/server/attack.c
        src/tests/unit/server/ban.c
        src/tests/unit/server/bank.c
        src/tests/unit/server/cache.c
        src/tests/unit/server/object.c
        src/tests/unit/server/re_cmp.c
        src/tests/unit/server/shop.c
        src/tests/unit/toolkit/math.c
        src/tests/unit/toolkit/memory.c
        src/tests/unit/toolkit/packet.c
        src/tests/unit/toolkit/pbkdf2.c
        src/tests/unit/toolkit/shstr.c
        src/tests/unit/toolkit/string.c
        src/tests/unit/toolkit/stringbuffer.c
        src/tests/unit/types/light_apply.c
        src/tests/unit/types/sound_ambient.c)
endif ()

# Find the world maker files, if enabled.
if (HAVE_WORLD_MAKER)
    set(SOURCES_WORLD_MAKER
        src/modules/world_maker.c)
endif ()

# Compile the sources into an executable.
add_executable(${EXECUTABLE} ${SOURCES} ${LEXERS_OUT} ${SOURCES_CHECK} ${SOURCES_WORLD_MAKER})
target_link_libraries(${EXECUTABLE} deusmagi-toolkit)
target_link_libraries(${EXECUTABLE} -Xlinker --allow-multiple-definition)

# Link libraries.
if (DL_LIBRARY)
    target_link_libraries(${EXECUTABLE} ${DL_LIBRARY})
endif ()

# Link check.
if (CHECK_LIBRARY)
    target_link_libraries(${EXECUTABLE} ${CHECK_LIBRARY})
    target_link_libraries(${EXECUTABLE} subunit)

    if (UNIX AND NOT APPLE)
        target_link_libraries(${EXECUTABLE} rt)
    endif ()
endif ()

# Link GD.
if (GD_LIBRARY)
    target_link_libraries(${EXECUTABLE} ${GD_LIBRARY})

    if (MINGW)
        target_link_libraries(${EXECUTABLE} png)
    endif ()
endif ()

# Get all the include directories used to compile the server.
get_directory_property(INCLUDES INCLUDE_DIRECTORIES)

# Go through the include directories and construct a string.
foreach (var ${INCLUDES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" var ${var})
    set(INCLUDES_STRING "${INCLUDES_STRING} -I${var}")
endforeach (var)

# Go through the source files and construct a string.
foreach (var ${SOURCES} ${LEXERS_OUT})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" var ${var})
    set(SOURCES_STRING "${SOURCES_STRING};${var}")
endforeach (var)

add_custom_target(test
    COMMAND ${EXECUTABLE} --unit --logger_filter_stdout=-info,-devel || exit 5
    COMMAND ${EXECUTABLE} --plugin_unit || exit 5
    COMMENT "Executing unit tests..."
    VERBATIM
)

if (CHECK_LIBRARY)
    # Go through the source files and construct a string.
    foreach (var ${SOURCES_CHECK})
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" var ${var})
        set(SOURCES_CHECK_STRING "${SOURCES_CHECK_STRING};${var}")
    endforeach (var)

    # Create a 'proto' build target, which generates the proto.h file.
    add_custom_target(proto_unit
        cproto -ve -o src/tests/check_proto.h.bak -D__CPROTO__${INCLUDES_STRING}${SOURCES_CHECK_STRING}
        COMMAND cmake -E echo "#ifndef __CPROTO__" > src/tests/check_proto.h
        COMMAND sed -e "s/\"\" //g" < src/tests/check_proto.h.bak >> src/tests/check_proto.h
        COMMAND cmake -E echo "#endif" >> src/tests/check_proto.h
        COMMAND cmake -E remove -f src/tests/check_proto.h.bak
        COMMAND python3 src/toolkit/adjust_proto.py src/tests/check_proto.h || exit 5
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Generating unit test prototypes..."
        VERBATIM)
endif ()

# Create the Arena plugin.
set(SOURCES_PLUGIN_ARENA
    src/plugins/plugin_arena/plugin_arena.c)
add_library(plugin_arena MODULE ${SOURCES_PLUGIN_ARENA})

# Set the preferred order of Python versions to find.
set(Python_ADDITIONAL_VERSIONS 3.10)
set(CMAKE_USE_PYTHON_VERSION ${Python_ADDITIONAL_VERSIONS})
# And try to find Python.
find_package(PythonLibs)

# Create the Python plugin, if Python was found.
if (PYTHONLIBS_FOUND)
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
        src/plugins/plugin_python/plugin_python.c)
    add_library(plugin_python MODULE ${SOURCES_PLUGIN_PYTHON})
    target_link_libraries(plugin_python ${PYTHON_LIBRARIES})
    target_link_libraries(plugin_python -Xlinker --allow-multiple-definition)
else ()
    message(STATUS "Python libs not found, Python plugin will not be built.")
endif ()

# Configure the .h file with the configuration options (size of long,
# package version, etc).
configure_file(src/include/cmake.h.def src/include/cmake.h)
configure_file(src/include/version.h.def src/include/version.h)

add_subdirectory(common/toolkit)