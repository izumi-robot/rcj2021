#include <Arduino.h>
#include "line_sensor.h"

int robo::LineSensor::white_border = 600;

bool robo::LineSensor::iswhite(int c)
{
    return c >= white_border;
}

void robo::LineSensor::setup()
{
    pinMode(in_pin, INPUT);
}

int robo::LineSensor::read()
{
    return analogRead(in_pin);
}
