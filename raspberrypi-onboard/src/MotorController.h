#pragma once

// constants 
#define PWM_FREQ_HZ             200
#define GPIO_PWM_THROTTLE       18     // PWM0 channel with HW timer
#define GPIO_IN1                23
#define GPIO_IN2                24

class MotorController
{
public:
    MotorController();
    ~MotorController();

    bool init(const int handle);
    void shutdown() const;
    void updateMotorThrottle(int throttle) const;

private:
    void setDriveDirectionForward() const;
    void setDriveDirectionBackward() const;
    void stopMotor() const;

private:
    int gpio_handle;
};