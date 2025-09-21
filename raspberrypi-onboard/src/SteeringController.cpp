/*
    file: SteeringController.cpp
    written by Elias Geiger
*/

#include "SteeringController.h"

#include <pigpiod_if2.h>
#include <iostream>

// definitions //

SteeringController::SteeringController()
{
    this->m_gpio_handle = -1;
}

SteeringController::~SteeringController()
{
    this->m_gpio_handle = -1;
}

bool SteeringController::init(const int handle)
{
    // store handle to interface with GPIO daemon
    this->m_gpio_handle = handle;

    if(set_mode(this->m_gpio_handle, GPIO_SERVO, PI_OUTPUT) != 0) {
        std::cerr << "[SteeringController] " << "Failed to configure gpio pin for servo!" << std::endl;
        return false;
    }

    // initialized steering servo in center position
    this->center_steering_servo();

    std::cout << "[SteeringController] " << "Steering controller initialized and ready" << std::endl;

    return true;
}

void SteeringController::shutdown() const
{
    // center steering servo before shutdown
    this->center_steering_servo();
}

void SteeringController::update_steering_angle(const int angle) const
{
    // map zero-centered angle values to the servo pulse duration range
    // optimized way for default limits
    int pulse_duration = ((100 * angle) / 51) + 1500;  

    // generic way for custom limits
    // int pulse_duration = (((angle + 255) * MIN_PULSE_DURATION) / 510) + MIN_PULSE_DURATION;

    // clamp the pulse duration to the allowed range (to protect servo from damage)
    if(pulse_duration > MAX_PULSE_DURATION) {
        pulse_duration = MAX_PULSE_DURATION;
    } else if(pulse_duration < MIN_PULSE_DURATION) {
        pulse_duration = MIN_PULSE_DURATION;
    }

    set_servo_pulsewidth(this->m_gpio_handle, GPIO_SERVO, pulse_duration);
}

void SteeringController::center_steering_servo() const
{
    set_servo_pulsewidth(this->m_gpio_handle, GPIO_SERVO, CENTER_PULSE_DURATION);
}
