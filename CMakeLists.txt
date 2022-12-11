cmake_minimum_required(VERSION 3.14)
project(pt)
add_executable("${PROJECT_NAME}" main.cpp)

# include Penning
add_subdirectory(src)
target_link_libraries("${PROJECT_NAME}" PUBLIC penning)
target_include_directories("${PROJECT_NAME}" PUBLIC
                        "${PROJECT_BINARY_DIR}"
                        "${PROJECT_SOURCE_DIR}/src")

# install binary
install(TARGETS "${PROJECT_NAME}" DESTINATION bin)