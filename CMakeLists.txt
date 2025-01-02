cmake_minimum_required(VERSION 3.27)

project(DiEventLib VERSION 0.1.0 DESCRIPTION "DiEvent/DvScene library" LANGUAGES CXX)
set(PROJECT_AUTHOR "Ashrindy, ik-01")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(DiEventLib STATIC)

target_sources(DiEventLib
    PRIVATE
        src/internal.cpp
        src/template/dievtdbbin.cpp
        src/template/dievtdb.cpp
    PUBLIC FILE_SET HEADERS FILES
        include/dieventlib/dvcommon.h
        include/dieventlib/dvdisableframe.h
        include/dieventlib/dvheader.h
        include/dieventlib/dvnode.h
        include/dieventlib/dvnodestructs.h
        include/dieventlib/dvobject.h
        include/dieventlib/dvpage.h
        include/dieventlib/dvresource.h
        include/dieventlib/dvscene.h
        include/dieventlib/internal.h
        include/dieventlib/memory.h
        include/dieventlib/template/dievtdb.h
        include/dieventlib/template/dievtdbbin.h
)

target_include_directories(DiEventLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)