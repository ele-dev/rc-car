/*
    file: GPIO.h
    written by Elias Geiger
*/

#pragma once

// includes
#include <pigpiod_if2.h>
#include <stdexcept>
#include <iostream>

class GPIO
{
public:
    GPIO();
    virtual ~GPIO();
    GPIO(const GPIO&) = delete;             // non-copyable
    GPIO& operator=(const GPIO&) = delete;  // non-assignable

    int get_handle() const;                 // handle to access GPIO from anywhere

private:
    int m_gpio_handle;
};