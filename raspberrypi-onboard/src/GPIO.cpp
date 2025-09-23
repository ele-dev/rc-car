/*
    file: GPIO.cpp
    written by Elias Geiger
*/

#include "GPIO.h"

// definitions //

GPIO::GPIO() 
{
    this->m_gpio_handle = -1;
    // connect to pigpio systemd daemon and retrieve handle
    this->m_gpio_handle = pigpio_start(NULL, NULL);
    if(this->m_gpio_handle < 0) {
        throw std::runtime_error("RuntimeError while initializing PiGPIO API!");
    }
}

virtual GPIO::~GPIO() 
{
    // close the connection to the daemon
    pigpio_stop(this->m_gpio_handle);
    this->m_gpio_handle = -1;
}

int GPIO::get_handle() const {
    return this->m_gpio_handle;
}