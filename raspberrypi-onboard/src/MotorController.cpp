#include "MotorController.h"

#include <pigpio.h>
#include <iostream>

// definitions //

MotorController::MotorController()
{
    std::cout << "created motor controller handle" << std::endl;
}

MotorController::~MotorController()
{
}

bool MotorController::init() 
{

    /*
    // initialise GPIO API 
    if(gpioInitialise() < 0) {
        std::cerr << "PiGPIO API init failed!" << std::endl;
        return false;
    }
    */

    // configure IN1 & IN2 as digital output channels
    if(gpioSetMode(GPIO_IN1, PI_OUTPUT) != 0) {
        std::cerr << "GPIO pin configuration failed!" << std::endl;
        return false;
    }

    if(gpioSetMode(GPIO_IN2, PI_OUTPUT) != 0) {
        std::cerr << "GPIO pin configuration failed!" << std::endl;
        return false;
    }

    // setup fixed frequency PWM signal channel for throttle control
    if(gpioSetPWMfrequency(GPIO_PWM_THROTTLE, PWM_FREQ_HZ) != 0) {
        std::cerr << "GPIO pwm configuration failed!" << std::endl;
        return false;
    }

    // set initial throttle to stand still
    stopMotor();

    std::cout << "motor controller initialized and ready" << std::endl;

    return true;
}

void MotorController::shutdown() 
{
    // reset throttle to stand still
    stopMotor();

    // terminate API handler
    gpioTerminate();
}

void MotorController::updateMotorThrottle(int throttle)
{
    // clamp the throtlle to the allowed range
    if(throttle > 255) {
        throttle = 255;
    } else if(throttle < -255) {
        throttle = -255;
    }

    // small throttle values below certain threshold shall not drive the motor (protect against noisy inputs)
    if(abs(throttle) < 30) {
        stopMotor();
        return;
    }

    // change motor drive direction based on the sign (forward + | backward -)
    if(throttle > 0) {
        setDriveDirectionForward();
    } else {
        setDriveDirectionBackward();
    }

    // output new duty cycle to ajdust motor throttle
    gpioPWM(GPIO_PWM_THROTTLE, abs(throttle));
}

void MotorController::setDriveDirectionForward()
{
    gpioWrite(GPIO_IN1, 0);
    gpioWrite(GPIO_IN2, 1);
}

void MotorController::setDriveDirectionBackward()
{
    gpioWrite(GPIO_IN1, 1);
    gpioWrite(GPIO_IN2, 0);
}

void MotorController::stopMotor()
{
    gpioWrite(GPIO_IN1, 0);
    gpioWrite(GPIO_IN2, 0);
    gpioPWM(GPIO_PWM_THROTTLE, 0);
}
