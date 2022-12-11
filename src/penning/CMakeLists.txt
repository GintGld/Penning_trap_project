add_library(penning
    penning.h
    penning.cpp
)

# include Phys and Matplot
include_directories("..")
include_directories("../matplotplusplus/source/")
target_link_libraries("${PROJECT_NAME}" INTERFACE phys)
target_link_libraries("${PROJECT_NAME}" PUBLIC penning)
target_link_libraries("${PROJECT_NAME}" PUBLIC matplot)
target_include_directories("${PROJECT_NAME}" PUBLIC
                        "${PROJECT_BINARY_DIR}"
                        "${PROJECT_SOURCE_DIR}/src")