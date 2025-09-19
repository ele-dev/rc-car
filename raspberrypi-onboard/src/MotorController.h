#pragma once

// constants 
#define PWM_FREQ_HZ             200
#define GPIO_PWM_THROTTLE       12     // use pin with HW PWM capabilities
#define GPIO_IN1                14
#define GPIO_IN2                15

class MotorController
{
public:
    MotorController();
    ~MotorController();

    bool init();
    void shutdown();
    void updateMotorThrottle(int throttle);

private:
    void setDriveDirectionForward();
    void setDriveDirectionBackward();
    void stopMotor();
};