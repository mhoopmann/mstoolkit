# Minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Project name and languages used
project(MSToolkit LANGUAGES CXX C)

IF (WIN32)
# Compiler options
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /sdl /Zc:inline /fp:precise /errorReport:prompt /WX- /Zc:forScope /Gd /Oi /MD /FC /EHsc /nologo /diagnostics:column")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /permissive- /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /sdl /Zc:inline /fp:precise /errorReport:prompt /WX- /Zc:forScope /Gd /Oi /MD /FC /EHsc /nologo /diagnostics:column")
# make sure to use the static runtime library for release build
#  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
# Definitions
  add_definitions(-DWIN32 -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE -DXML_STATIC -D_MBCS -D_NOSQLITE)
ENDIF()

IF (UNIX)
  # for linux
  add_definitions(-DGCC -DXML_DEV_URANDOM -D_FILE_OFFSET_BITS=64 -D_NO_THERMORAW -D_NOSQLITE)
ENDIF()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source directories
set(MSTOOLKIT_SRC_DIR ${PROJECT_SOURCE_DIR}/src/MSToolkit)
set(EXPAT_SRC_DIR ${PROJECT_SOURCE_DIR}/src/expat-2.2.9/lib)
set(MZID_SRC_DIR ${PROJECT_SOURCE_DIR}/src/mzIMLTools)
set(MZP_SRC_DIR ${PROJECT_SOURCE_DIR}/src/mzParser)
set(ZLIB_SRC_DIR ${PROJECT_SOURCE_DIR}/src/zlib-1.2.11)

# Library output directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# Add library targets
file(GLOB MSTOOLKIT_SOURCES "${MSTOOLKIT_SRC_DIR}/*.cpp")
add_library(mstoolkit STATIC ${MSTOOLKIT_SOURCES})
file(GLOB EXPAT_SOURCES "${EXPAT_SRC_DIR}/*.c")
add_library(expat STATIC ${EXPAT_SOURCES})
file(GLOB MZID_SOURCES "${MZID_SRC_DIR}/*.cpp")
add_library(mzid STATIC ${MZID_SOURCES})
file(GLOB MZP_SOURCES "${MZP_SRC_DIR}/*.cpp")
add_library(mzparser STATIC ${MZP_SOURCES})
file(GLOB ZLIB_SOURCES "${ZLIB_SRC_DIR}/*.c")
add_library(zlib STATIC ${ZLIB_SOURCES})

# Link libraries
target_link_libraries(mzparser expat)
target_link_libraries(mstoolkit expat mzid mzparser zlib)
