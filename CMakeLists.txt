cmake_minimum_required(VERSION 3.10)

project(pt)

add_executable("${PROJECT_NAME}" main.cpp)

add_subdirectory(penning REQUIRED)
find_package(Matplot++ REQUIRED)

target_link_libraries("${PROJECT_NAME}" PUBLIC penning)
target_link_libraries("${PROJECT_NAME}" PUBLIC Matplot++::matplot)

target_include_directories("${PROJECT_NAME}" PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/penning"
                          )

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)