#include "MotorController.h"

#include <pigpiod_if2.h>
#include <iostream>

// definitions //

MotorController::MotorController()
{
    this->gpio_handle = NULL;
    std::cout << "Created motor controller handle" << std::endl;
}

MotorController::~MotorController()
{
    this->gpio_handle = NULL;
}

bool MotorController::init(const int handle) 
{
    // store handle to interface with GPIO daemon inside class
    this->gpio_handle = handle;

    // configure IN1 & IN2 as digital output channels
    if(set_mode(this->gpio_handle, GPIO_IN1, PI_OUTPUT) != 0) {
        std::cerr << "Failed to configure gpio pin IN1!" << std::endl;
        return false;
    }

    if(set_mode(this->gpio_handle, GPIO_IN2, PI_OUTPUT) != 0) {
        std::cerr << "Failed to configure gpio pin IN2!" << std::endl;
        return false;
    }

    // setup fixed frequency PWM signal channel for throttle control
    if(set_PWM_frequency(this->gpio_handle, GPIO_PWM_THROTTLE, PWM_FREQ_HZ) != 0) {
        std::cerr << "Failed to configure PWM for gpio pin EN!" << std::endl;
        return false;
    }

    // set initial throttle to stand still
    this->stopMotor();

    std::cout << "Motor controller initialized and ready" << std::endl;

    return true;
}

void MotorController::shutdown() const
{
    // reset throttle to stand still
    this->stopMotor();
}

void MotorController::updateMotorThrottle(int throttle) const
{
    // clamp the throtlle to the allowed range
    if(throttle > 255) {
        throttle = 255;
    } else if(throttle < -255) {
        throttle = -255;
    }

    // small throttle values below certain threshold shall not drive the motor (protect against noisy inputs)
    if(abs(throttle) < 30) {
        this->stopMotor();
        return;
    }

    // change motor drive direction based on the sign (forward + | backward -)
    if(throttle > 0) {
        this->setDriveDirectionForward();
    } else {
        this->setDriveDirectionBackward();
    }

    // output new duty cycle to ajdust motor throttle
    set_PWM_dutycycle(this->gpio_handle, GPIO_PWM_THROTTLE, abs(throttle));
}

void MotorController::setDriveDirectionForward() const
{
    gpio_write(this->gpio_handle, GPIO_IN1, 0);
    gpio_write(this->gpio_handle, GPIO_IN2, 1);
}

void MotorController::setDriveDirectionBackward() const 
{
    gpio_write(this->gpio_handle, GPIO_IN1, 1);
    gpio_write(this->gpio_handle, GPIO_IN2, 0);
}

void MotorController::stopMotor() const 
{
    gpio_write(this->gpio_handle, GPIO_IN1, 0);
    gpio_write(this->gpio_handle, GPIO_IN2, 0);
    set_PWM_dutycycle(this->gpio_handle, GPIO_PWM_THROTTLE, 0);
}
