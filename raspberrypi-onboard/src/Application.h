/*
    file: Application.h
    written by Elias Geiger
*/

#pragma once

// includes
#include "GPIO.h"
#include "MotorController.h"
#include "SteeringController.h"
#include "Gamepad.h"


class Application 
{
public:
    Application();
    virtual ~Application();
    Application(const Application&) = delete;               // non-copyable
    Application& operator=(const Application&) = delete;    // non-assignable

    void run_app(bool& running_flag);

private:
    // sub modules (in depency order)
    GPIO                        m_gpio;
    MotorController             m_motor{m_gpio};
    SteeringController          m_steering_wheel{m_gpio};
    Gamepad                     m_gamepad;
};