#include "SteeringController.h"

#include <pigpio.h>
#include <iostream>

// definitions //

SteeringController::SteeringController()
{
}

SteeringController::~SteeringController()
{
    shutdown();
}

bool SteeringController::init()
{
    // initialise GPIO API 
    if(gpioInitialise() < 0) {
        std::cerr << "PiGPIO API init failed!" << std::endl;
        return false;
    }

    // initialized steering servo in center position
    centerSteeringServo();

    return true;
}

void SteeringController::shutdown()
{
    // center steering servo before shutdown
    centerSteeringServo();

    // terminate API handler
    gpioTerminate();
}

void SteeringController::updateSteeringAngle(int angle)
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

    gpioServo(GPIO_SERVO, pulse_duration);
}

void SteeringController::centerSteeringServo()
{
    gpioServo(GPIO_SERVO, CENTER_PULSE_DURATION);
}
