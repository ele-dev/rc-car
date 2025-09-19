#pragma once

#define GPIO_SERVO                  17
#define MIN_PULSE_DURATION          1000    // hard left turn
#define CENTER_PULSE_DURATION       1500    // straight
#define MAX_PULSE_DURATION          2000    // hard right turn

class SteeringController 
{
public:
    SteeringController();
    ~SteeringController();

    bool init(const int handle);
    void shutdown() const;
    void updateSteeringAngle(const int angle) const;

private:
    void centerSteeringServo() const;

private:
    int gpio_handle;
};