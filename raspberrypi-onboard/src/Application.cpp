/*
    file: Application.cpp
    written by Elias Geiger
*/

#include "Application.h"

// definitions //

Application::Application()
{
}

Application::~Application()
{
}

void Application::run_app(bool& running_flag) 
{
    // init gamepad 
    bool success = m_gamepad.init();
    if(!success) {
        return;
    }

    static int last_throttle_cmd = 0;
    static int last_steering_cmd = 0;

    // main application loop
    std::cout << "\nBegin main processing loop ..." << std::endl;
    while(running_flag)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e)) 
        {
            switch(e.type)
            {
                case SDL_CONTROLLERAXISMOTION:
                {
                    // general joystick input preprocessing 
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

                    // motor drive
                    if(e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                        // calculate updated throttle value
                        int throttle_cmd = (input_value * -255) / 32767;

                        // only send if throttle_cmd has changed
                        if(throttle_cmd != last_throttle_cmd) {
                            this->m_motor.updateMotor_throttle(throttle_cmd);
                            last_throttle_cmd = throttle_cmd;
                            std::cout << "--> Sent throttle command of " << throttle_cmd << std::endl;
                        }
                        break;
                    }

                    // steering wheel (servo) 
                    if(e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
                        // calculate updated steering command
                        int steering_cmd = (input_value * 255) / 32767;

                        // only send if steering command has changed
                        if(steering_cmd != last_steering_cmd) {
                            this->m_steering_wheel.update_steering_angle(steering_cmd);
                            last_steering_cmd = steering_cmd;
                            std::cout << "--> Sent steering command of " << steering_cmd << std::endl;
                        }
                        break;
                    }

                    // debug for movements on any other axis
                    // std::cout << "Movement event on axis " << SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(e.caxis.axis)) << std::endl;

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
                case SDL_CONTROLLERDEVICEREMOVED:
                {
                    if(e.cdevice.which == this->m_gamepad.get_instance_id()) {
                        std::cout << "[Warning] Gamepad connection lost! --> Emergency stop" << std::endl;
                        this->m_motor.updateMotor_throttle(0);
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

    // close gamepad
    this->m_gamepad.shutdown();
}
