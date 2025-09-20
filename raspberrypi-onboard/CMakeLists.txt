cmake_minimum_required(VERSION 3.10)

# Setup the project
project(rc-car-controller LANGUAGES CXX)

# Set the bin folder
set(EXECUTABLE_OUTPUT_PATH "../bin")

# Set the C++ language standard
set(CMAKE_CXX_STANDARD 17)

# Add your source files here
set(SOURCES
    src/main.cpp 
    src/MotorController.cpp
    src/SteeringController.cpp
    src/Gamepad.cpp
)

# Add any additional include directories
include_directories(
    include
)

# Find packages 
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Add all external libraries    
find_library(PIGPIO_LIB pigpiod_if2 REQUIRED)          
find_library(PTHREAD_LIB pthread REQUIRED)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add additional include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

# Link the necessary libraries
target_link_libraries(${PROJECT_NAME}
    ${PIGPIO_LIB}
    ${PTHREAD_LIB}
    ${SDL2_LIBRARIES}
)