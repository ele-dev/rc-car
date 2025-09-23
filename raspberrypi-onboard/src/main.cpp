/*
    file: main.cpp
    written by Elias Geiger
*/

#include "Application.h"

#include <iostream>
#include <signal.h>
#include <chrono>
#include <thread>

// function prototypes
void terminate_signal_handler(int);
// void test_motor_control();

// application handle
static bool running = true;
Application app_handle;

// ------------ application main section ------------------- //

int main(int argc, char **argv)
{
    std::cout << "Launch RC Car controller application" << std::endl;

    // create signal handler for clean Ctrl+C close up
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = terminate_signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, nullptr);

    // execute the actual application logic
    app_handle.run_app(running);

    return EXIT_SUCCESS;
}

void terminate_signal_handler(int code) {
    std::cout << "\nQuit application via Ctrl+C ...\n";
    // exit(code);

    // unset running flag to leave application loop in main context
    running = false;
}

/*
void test_motor_control()
{
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
}
*/