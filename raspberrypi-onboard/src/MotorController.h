/*
    file: MotorController.h
    written by Elias Geiger
*/

#pragma once

// includes 
#include "GPIO.h"

#include <iostream>

// constants 
#define PWM_FREQ_HZ                 200
#define CENTER_DEADZONE_THRESHOLD   70
#define GPIO_PWM_THROTTLE           18  // PWM0 channel with HW timer
#define GPIO_IN1                    23
#define GPIO_IN2                    24

class MotorController
{
public:
    MotorController(GPIO& gpio_ptr);
    virtual ~MotorController();

    bool init();
    void shutdown() const;
    void updateMotor_throttle(int throttle) const;

private:
    void set_drive_direction_forward() const;
    void set_drive_direction_backward() const;
    void stop_motor() const;

private:
    // int m_gpio_handle;
    GPIO& m_gpio;
};