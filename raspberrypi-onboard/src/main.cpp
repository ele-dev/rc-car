/*
    file: main.cpp
    written by Elias Geiger
*/

#include "MotorController.h"
#include "SteeringController.h"

#include <iostream>
#include <signal.h>

// function prototypes
void terminateSignalHandler(int);

// submodule instances
MotorController motor;
SteeringController steeringwheel;
int gpio_handle = NULL;

// ------------ application main section ------------------- //

int main(int argc, char **argv)
{
    std::cout << "Launch RC Car controller application" << std::endl;

    // create signal handler for clean Ctrl+C close up
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = terminateSignalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    // connect to pigpio systemd daemon and retrieve handle
    gpio_handle = pigpio_start(NULL, NULL);

    // init submodules
    bool result = steeringwheel.init(gpio_handle);
    if(!result) {
        exit(EXIT_FAILURE);
    }

    result = motor.init(gpio_handle);
    if(!result) {
        exit(EXIT_FAILURE);
    }

    // infinite loop
    while(true)
    {
        // ...
    }

    terminateSignalHandler(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

void terminateSignalHandler(int code) {
    // do manual cleanup tasks
    motor.shutdown();
    steeringwheel.shutdown();

    // close the connection to the daemon
    pigpio_stop(gpio_handle);

    std::cout << "\nExit now." << std::endl;

    exit(code);
}