cmake_minimum_required(VERSION 3.6)
project(valhalla_vm)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    src/valhalla/Assembler.cpp
    src/valhalla/Assembler.h
    src/valhalla/Display.cpp
    src/valhalla/Display.h
    src/valhalla/instruction_definitions.h
    src/valhalla/register_definitions.h
    src/valhalla/VM.cpp
    src/valhalla/VM.h
    src/data_types.h
    src/main.cpp
    src/math.h)

add_executable(valhalla_vm ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(valhalla_vm ${SFML_LIBRARIES})
endif()