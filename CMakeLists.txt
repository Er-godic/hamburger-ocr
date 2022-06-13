cmake_minimum_required(VERSION 3.16)
project(hamburger-ocr)

#set
set(CMAKE_CXX_STANDARD 17)
set(src "${PROJECT_SOURCE_DIR}/src")
set(EXECUTABLE_OUTPUT_PATH "../bin/")

# package / lib
link_directories(/usr/local/lib /usr/lib)
find_package(X11 REQUIRED)

#add source files
file(GLOB_RECURSE source_files "${src}/*.cpp")

#add the executable 
add_executable(${PROJECT_NAME} ${source_files}) 

target_compile_definitions(${PROJECT_NAME} PRIVATE HAMBURGER_DATA_PATH="${PROJECT_SOURCE_DIR}/data/")

#include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${src} "stb" 
	${X11_INCLUDE_DIRS})

#link
target_link_libraries(${PROJECT_NAME} PRIVATE tesseract lept
	${X11_LIBRARIES})

