cmake_minimum_required(VERSION 3.5)

# --- Project data ---------------------------------------------------------------------
project(pepon
        VERSION 1.0.0.0
        DESCRIPTION "C++ Object Notation file format/reading/writing library for modern C++(17+)"
        LANGUAGES CXX)


# --- Project requirements -------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# --- Project configuration ------------------------------------------------------------
# Set library name
set(PEPON_LIBRARY_NAME "PEPON")

# Set library description string
set(PEPON_DESCRIPTION "${pepon_DESCRIPTION}")

# Set library version number
set(PEPON_VERSION_MAJOR_INT "${pepon_VERSION_MAJOR}")
set(PEPON_VERSION_MINOR_INT "${pepon_VERSION_MINOR}")
set(PEPON_VERSION_PATCH_INT "${pepon_VERSION_PATCH}")
set(PEPON_VERSION_TWEAK_INT "${pepon_VERSION_TWEAK}")
set(PEPON_VERSION_FULL_STRING "${pepon_VERSION}")

# Set DEBUG flag
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(PEPON_DEBUG 1)
endif()

# Use STL path header?
set(PEPON_USE_STL_PATH 1)

# Configure a header file to pass the CMake settings to the source code
# This file MUST be generated in the build folder
configure_file(
    "${pepon_SOURCE_DIR}/cmake/PeponConfig.hpp.in"
    "${pepon_BINARY_DIR}/PeponConfig.hpp"
)


# --- Add submodules -------------------------------------------------------------------
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${pepon_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)

    if(GIT_SUBMODULE)
        message(STATUS "Updating git submodules")
        # This will update the submodules, and if they're not initiated yet, will initiate them
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)

        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()

    endif()
endif()

# Check if submodules operations were successful
if(NOT EXISTS "${pepon_SOURCE_DIR}/extLibs/myoga-utils/README.md")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again")
endif()


# --- Compiler configuration -----------------------------------------------------------
# Compiler flags/options
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    # Clang / GNU use the same flags
    set(PEPON_COMPILE_OPTIONS "-Wall;-Wextra")
    set(PEPON_COMPILE_DEBUG_OPTIONS   "${PEPON_COMPILE_OPTIONS};-Og")
    set(PEPON_COMPILE_RELEASE_OPTIONS "${PEPON_COMPILE_OPTIONS};-O2")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Visual Studio flags
    set(PEPON_COMPILE_OPTIONS "/EHsc /W4")
    set(PEPON_COMPILE_DEBUG_OPTIONS   "${PEPON_COMPILE_OPTIONS};/Zi")
    set(PEPON_COMPILE_RELEASE_OPTIONS "${PEPON_COMPILE_OPTIONS};/O2;/subsystem:windows")
endif()


# --- Project files --------------------------------------------------------------------
set(PEPON_HEADER_FILES
    # extLibs/myoga-utils/Utils
    extLibs/myoga-utils/Utils/MapFinder.hpp
    extLibs/myoga-utils/Utils/StringUtils.hpp
    extLibs/myoga-utils/Utils/TemplateUtils.hpp
    # peponLib/Variables
    peponLib/Variables/IVariable.hpp
    peponLib/Variables/SingleValue.hpp
    peponLib/Variables/VectorValue.hpp
    peponLib/Variables/DequeValue.hpp
    peponLib/Variables/ListValue.hpp
    peponLib/Variables/Group.hpp
    # peponLib
    peponLib/Pepon.hpp
    peponLib/PeponFile.hpp
    )

set(PEPON_SOURCE_FILES
    # extLibs/myoga-utils/Utils
    extLibs/myoga-utils/Utils/StringUtils.cpp
    # peponLib/Variables
    peponLib/Variables/IVariable.cpp
    peponLib/Variables/SingleValue.cpp
    peponLib/Variables/Group.cpp
    # peponLib
    peponLib/Pepon.cpp
    peponLib/PeponFile.cpp
    )


# --- Output binary files --------------------------------------------------------------
# --- Pepon Library --------------------------------------------------------------------
# Set library target name
set(PEPON_LIBRARY_TARGET "Pepon-StaticLib_${CMAKE_BUILD_TYPE}")
# Set output folder
set(PEPON_LIBRARY_BINARY_FOLDER "${pepon_SOURCE_DIR}/bin")

# Compile as static library
add_library(${PEPON_LIBRARY_TARGET} STATIC)
# Set static library output directory
set_target_properties(${PEPON_LIBRARY_TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PEPON_LIBRARY_BINARY_FOLDER})

# Add header and source files
target_sources(${PEPON_LIBRARY_TARGET} PRIVATE ${PEPON_HEADER_FILES} PRIVATE ${PEPON_SOURCE_FILES})

# Include folders:
# Easy access to the config file CMake generated for us
target_include_directories(${PEPON_LIBRARY_TARGET} PUBLIC ${pepon_BINARY_DIR})
# Source folder
target_include_directories(${PEPON_LIBRARY_TARGET} PUBLIC ${pepon_SOURCE_DIR})
# extLibs folder
target_include_directories(${PEPON_LIBRARY_TARGET} PUBLIC "${pepon_SOURCE_DIR}/extLibs")

# Compiler options
target_compile_options(${PEPON_LIBRARY_TARGET} PUBLIC "$<$<CONFIG:DEBUG>:${PEPON_COMPILE_DEBUG_OPTIONS}>")
target_compile_options(${PEPON_LIBRARY_TARGET} PUBLIC "$<$<CONFIG:RELEASE>:${PEPON_COMPILE_RELEASE_OPTIONS}>")


# --- Test/example program -------------------------------------------------------------
# Target and source file
set(PEPON_TEST pepon_test)
add_executable(${PEPON_TEST} "${pepon_SOURCE_DIR}/tests/${PEPON_TEST}.cpp")

# Link it against the Pepon library
target_link_libraries(${PEPON_TEST} PRIVATE ${PEPON_LIBRARY_TARGET})

# Output directory for executable
set_target_properties(${PEPON_TEST} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PEPON_LIBRARY_BINARY_FOLDER})
