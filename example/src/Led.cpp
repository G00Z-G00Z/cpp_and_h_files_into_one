#include <Led.h>
#include <Arduino.h>

Led::Led() : pin(0) {}

Led::Led(int pin) : pin(pin) { pinMode(pin, OUTPUT); }

void Led::on() { digitalWrite(pin, HIGH); }
void Led::off() { digitalWrite(pin, LOW); }