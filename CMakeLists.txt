cmake_minimum_required(VERSION 3.14)
project(pt)
add_executable("${PROJECT_NAME}" main.cpp)

# inclide Matplot++
find_package(Matplot++ QUIET)
    if (Matplot++_FOUND)
    message(STATUS "Matplot++ library was found")
target_link_libraries("${PROJECT_NAME}" PUBLIC Matplot++::matplot)
else()
    add_subdirectory(matplotplusplus)
    target_link_libraries("${PROJECT_NAME}" PUBLIC matplot)
    message(STATUS "Matplot++ library was found")
endif()

# include Phys
add_subdirectory(src)
target_link_libraries("${PROJECT_NAME}" INTERFACE phys)
target_include_directories("${PROJECT_NAME}" PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/src")

# install binary
install(TARGETS "${PROJECT_NAME}" DESTINATION bin)