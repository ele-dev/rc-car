/*
    file: main.cpp
    written by Elias Geiger
*/

#include "MotorController.h"
#include "SteeringController.h"

#include <pigpiod_if2.h>
#include <iostream>
#include <signal.h>
#include <chrono>
#include <thread>

// function prototypes
void terminate_signal_handler(int);

// submodule instances
MotorController motor;
SteeringController steeringwheel;
int gpio_handle = -1;

// ------------ application main section ------------------- //

int main(int argc, char **argv)
{
    std::cout << "Launch RC Car controller application" << std::endl;

    // create signal handler for clean Ctrl+C close up
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = terminate_signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    // connect to pigpio systemd daemon and retrieve handle
    gpio_handle = pigpio_start(NULL, NULL);
    if(gpio_handle < 0) {
        std::cerr << "Failed to connect to PiGPIO systemd daemon!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // init submodules
    bool result = steeringwheel.init(gpio_handle);
    if(!result) {
        exit(EXIT_FAILURE);
    }

    result = motor.init(gpio_handle);
    if(!result) {
        exit(EXIT_FAILURE);
    }

    // main application loop
    int throttle_cmd = 0;
    while(true)
    {
        motor.updateMotor_throttle(throttle_cmd);
        std::cout << "--> Sent motor throttle command: " << throttle_cmd << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(10));
        
        // increase and invert throttle command until upper boundaries are hit, then start over at zero
        if(throttle_cmd < 0) {
            throttle_cmd -= 25;
        } else {
            throttle_cmd += 25;
        }
        throttle_cmd *= -1;
        if(abs(throttle_cmd) > 255) {
            throttle_cmd = 0;
        }
    }

    terminate_signal_handler(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

void terminate_signal_handler(int code) {
    // do manual cleanup tasks
    motor.shutdown();
    steeringwheel.shutdown();

    // close the connection to the daemon
    pigpio_stop(gpio_handle);

    std::cout << "\nExit now." << std::endl;

    exit(code);
}