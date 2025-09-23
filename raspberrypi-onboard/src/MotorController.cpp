/*
    file: MotorController.cpp
    written by Elias Geiger
*/

#include "MotorController.h"

// definitions //

MotorController::MotorController(GPIO& gpio_ptr) : m_gpio(gpio_ptr)
{
    // this->m_gpio_handle = -1;
}

virtual MotorController::~MotorController()
{
    // this->m_gpio_handle = -1;
}

bool MotorController::init() 
{
    // store handle to interface with GPIO daemon inside class
    // this->m_gpio_handle = handle;

    // configure IN1 & IN2 as digital output channels
    if(set_mode(this->m_gpio.get_handle(), GPIO_IN1, PI_OUTPUT) != 0) {
        std::cerr << "[MotorController] " << "Failed to configure gpio pin IN1 as output!" << std::endl;
        return false;
    }

    if(set_mode(this->m_gpio.get_handle(), GPIO_IN2, PI_OUTPUT) != 0) {
        std::cerr << "[MotorController] " << "Failed to configure gpio pin IN2 as output!" << std::endl;
        return false;
    }

    // setup fixed frequency PWM signal channel for throttle control
    if(set_mode(this->m_gpio.get_handle(), GPIO_PWM_THROTTLE, PI_OUTPUT) != 0) {
        std::cerr << "[MotorController] " << "Failed tp configure EN pin as output!" << std::endl;
        return false;
    }

    int frequency = set_PWM_frequency(this->m_gpio.get_handle(), GPIO_PWM_THROTTLE, PWM_FREQ_HZ);
    if(frequency == PI_BAD_USER_GPIO || frequency == PI_NOT_PERMITTED) {
        std::cerr << "[MotorController] " << "Failed to configure PWM for gpio pin EN!" << std::endl;
        return false;
    } else {
        std::cout << "[MotorController] " << "Set PWM frequency for throttle signal to " << frequency << " Hz" << std::endl;
    }

    // set initial throttle to stand still
    this->stop_motor();

    std::cout << "[MotorController] " << "Motor controller initialized and ready" << std::endl;

    return true;
}

void MotorController::shutdown() const
{
    // reset throttle to stand still
    this->stop_motor();
}

void MotorController::updateMotor_throttle(int throttle) const
{
    // clamp the throtlle to the allowed range
    if(throttle > 255) {
        throttle = 255;
    } else if(throttle < -255) {
        throttle = -255;
    }

    // small throttle values below certain threshold shall not drive the motor (protect against noisy inputs)
    if(abs(throttle) < CENTER_DEADZONE_THRESHOLD) {
        this->stop_motor();
        return;
    }

    // change motor drive direction based on the sign (forward + | backward -)
    if(throttle > 0) {
        this->set_drive_direction_forward();
    } else {
        this->set_drive_direction_backward();
    }

    // output new duty cycle to ajdust motor throttle
    set_PWM_dutycycle(this->m_gpio.get_handle(), GPIO_PWM_THROTTLE, abs(throttle));
}

void MotorController::set_drive_direction_forward() const
{
    gpio_write(this->m_gpio.get_handle(), GPIO_IN1, 1);
    gpio_write(this->m_gpio.get_handle(), GPIO_IN2, 0);
}

void MotorController::set_drive_direction_backward() const 
{
    gpio_write(this->m_gpio.get_handle(), GPIO_IN1, 0);
    gpio_write(this->m_gpio.get_handle(), GPIO_IN2, 1);
}

void MotorController::stop_motor() const 
{
    gpio_write(this->m_gpio.get_handle(), GPIO_IN1, 0);
    gpio_write(this->m_gpio.get_handle(), GPIO_IN2, 0);
    set_PWM_dutycycle(this->m_gpio.get_handle(), GPIO_PWM_THROTTLE, 0);
}
