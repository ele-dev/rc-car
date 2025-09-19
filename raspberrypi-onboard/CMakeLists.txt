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
)

# Add any additional include directories
include_directories(
    include
)

# Add all external libraries
# find_library(WIRINGPI_LIB wiringPi REQUIRED)     
find_library(PIGPIO_LIB pigpiod_if2 REQUIRED)          
find_library(PTHREAD_LIB pthread REQUIRED)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the necessary libraries
target_link_libraries(${PROJECT_NAME}
    # ${WIRINGPI_LIB}
    ${PIGPIO_LIB}
    ${PTHREAD_LIB}
)