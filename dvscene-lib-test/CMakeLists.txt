project("dvscene-lib-test" VERSION 0.0.1)
add_executable(dvscene-lib-test
    "src/dvscene-lib-test.cpp" 
)
target_include_directories(dvscene-lib-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(dvscene-lib-test DiEventLib)
target_compile_features(dvscene-lib-test PRIVATE cxx_std_20)
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
