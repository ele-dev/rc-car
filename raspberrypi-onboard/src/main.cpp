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

    // init submodules
    bool result;
    result = steeringwheel.init();
    if(!result) {
        exit(EXIT_FAILURE);
    }

    result = motor.init();
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

    std::cout << "\nExit now." << std::endl;

    exit(code);
}