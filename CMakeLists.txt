cmake_minimum_required(VERSION 3.27)

if(CMAKE_BUILD_TYPE STREQUAL "Release")	
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Os -fdata-sections -ffunction-sections -fno-ident" CACHE STRING "C++ Compiler flags" FORCE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm -fPIC -fno-ident -s -Wl,--gc-sections" CACHE STRING "EXE link flags" FORCE)
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -g3 -Wall " CACHE STRING "C++ Compiler flags" FORCE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm " CACHE STRING "EXE link flags" FORCE)
    endif()
endif()

add_subdirectory("dvscene-lib")

set(DVSCENE_LIB_TEST False CACHE STRING "Build the test app for the lib")

if(DVSCENE_LIB_TEST)
    add_subdirectory("dvscene-lib-test")
endif()
