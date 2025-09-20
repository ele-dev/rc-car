/*
    file: main.cpp
    written by Elias Geiger
*/

#include "MotorController.h"
#include "SteeringController.h"
#include "Gamepad.h"

#include <pigpiod_if2.h>
#include <iostream>
#include <signal.h>
#include <chrono>
#include <thread>

// function prototypes
void terminate_signal_handler(int);
void test_motor_control();

// submodule instances
MotorController motor;
SteeringController steeringwheel;
Gamepad gamepad;
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

    result = gamepad.init();
    if(!result) {
        exit(EXIT_FAILURE);
    }

    // just for testing
    // test_motor_control();

    // main application loop
    std::cout << "\nBegin main processing loop ..." << std::endl;
    bool running = true;
    static int last_throttle_cmd = 0;
    while(running)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e)) 
        {
            switch(e.type)
            {
                case SDL_CONTROLLERAXISMOTION:
                {
                    if(e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                        int input_value = e.caxis.value;

                        // clamp raw values to expected range
                        if(input_value > 32768) {
                            input_value = 32768;
                        } else if(input_value < -32768) {
                            input_value = -32768;
                        }

                        // intentional joystick center deadzone (to protect against noise and stick drift)
                        if(abs(input_value) < 6000) {
                            input_value = 0;
                        }
                        
                        // calculate updated throttle value
                        int throttle_cmd = (input_value * -255) / 32767;

                        if(throttle_cmd != last_throttle_cmd) {
                            motor.updateMotor_throttle(throttle_cmd);
                            last_throttle_cmd = throttle_cmd;
                            std::cout << "--> Sent throttle command of " << throttle_cmd << std::endl;
                        }
                    }
                    break;
                }
                case SDL_CONTROLLERBUTTONDOWN:
                {
                    break;
                }
                case SDL_CONTROLLERBUTTONUP:
                {
                    break;
                }
                case SDL_CONTROLERDEVICEREMOVED:
                {
                    if(e.cdevice.which == gamepad.get_instance_id()) {
                        std::cout << "[Warning] Gamepad connection lost! --> Emergency stop" << std::endl;
                        motor.updateMotor_throttle(0);
                    }
                    break;
                }
                default:
                {
                    // std::cerr << "Unknown SDL event (will be ignored)" << std::endl;
                    break;
                }
            }
        }

        // short blocking idle time to reduce cpu load
        SDL_Delay(5);
    }

    terminate_signal_handler(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

void terminate_signal_handler(int code) {
    // do manual cleanup tasks
    motor.shutdown();
    steeringwheel.shutdown();
    gamepad.shutdown();

    // close the connection to the daemon
    pigpio_stop(gpio_handle);

    std::cout << "\nExit now." << std::endl;

    exit(code);
}

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
