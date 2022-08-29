#pragma once

/**
 * @brief Led class for handling on and off state
 *
 */
class Led
{

private:
    int pin;

public:
    Led();
    Led(int pin);
    void on();
    void off();
};