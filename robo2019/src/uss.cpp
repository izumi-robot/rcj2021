#include <Arduino.h>
#include "echo.h"

void robo::USSensor::setup()
{
    pinMode(input_pin, INPUT);
    pinMode(order_pin, OUTPUT);
}

int robo::USSensor::read()
{
    digitalWrite(order_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(order_pin, LOW);
    return pulseIn(input_pin, HIGH) / 59;
}