/*
    file: SteeringController.h
    written by Elias Geiger
*/

#pragma once

// includes 
#include "GPIO.h"

#include <iostream>

#define GPIO_SERVO                  17      // SW PWM for servo
#define MIN_PULSE_DURATION          1000    // hard left turn
#define CENTER_PULSE_DURATION       1500    // straight
#define MAX_PULSE_DURATION          2000    // hard right turn

class SteeringController 
{
public:
    SteeringController(GPIO& gpio_ptr);
    virtual ~SteeringController();

    bool init();
    void shutdown() const;
    void update_steering_angle(const int angle) const;

private:
    void center_steering_servo() const;

private:
    // int m_gpio_handle;
    GPIO& m_gpio;
};