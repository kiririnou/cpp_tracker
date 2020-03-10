cmake_minimum_required(VERSION 3.13)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_GENERATOR "MinGW Makefiles")
set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++ -Wcpp -DCURL_STATICLIB")

add_executable(tracker main.cpp utils.h)
set_target_properties(tracker PROPERTIES 
    OUTPUT_NAME ${CMAKE_PROJECT_NAME} 
    SUFFIX ".exe"
)

target_include_directories(tracker PUBLIC 
    ${PROJECT_SOURCE_DIR}/vendor/curl-7.69.0-win64-mingw/include
    ${PROJECT_SOURCE_DIR}/vendor/rapidjson/include
)

target_link_libraries(tracker PUBLIC 
    ${PROJECT_SOURCE_DIR}/vendor/curl-7.69.0-win64-mingw/lib/libcurl.dll.a
)