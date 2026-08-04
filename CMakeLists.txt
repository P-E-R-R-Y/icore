cmake_minimum_required(VERSION 3.14)
project(icore CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header-only : rien a compiler, juste un chemin d'include a propager.
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Les exemples ne se construisent que si icore est le projet principal.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  foreach(sample cli app)
    add_executable(icore_${sample} examples/${sample}.cpp)
    target_link_libraries(icore_${sample} PRIVATE ${PROJECT_NAME})
  endforeach()
endif()
