cmake_minimum_required(VERSION 3.14)
project(pt)
add_executable("${PROJECT_NAME}" main.cpp)

# include Phys
add_subdirectory(src)
target_link_libraries("${PROJECT_NAME}" INTERFACE phys)
target_link_libraries("${PROJECT_NAME}" INTERFACE penning)
target_include_directories("${PROJECT_NAME}" PUBLIC
                        "${PROJECT_BINARY_DIR}"
                        "${PROJECT_SOURCE_DIR}/src")

# inclide Matplot++
find_package(Matplot++ QUIET)
if (Matplot++_FOUND)
    target_link_libraries("${PROJECT_NAME}" PUBLIC Matplot++::matplot)
else()
    add_subdirectory(src/matplotplusplus)
    target_link_libraries("${PROJECT_NAME}" PUBLIC matplot)
endif()


# install binary
install(TARGETS "${PROJECT_NAME}" DESTINATION bin)