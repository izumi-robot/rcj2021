#include <Arduino.h>
#include "interrupt.h"

template<int in_pin, int mode>
volatile bool robo::Interrupt<in_pin, mode>::_state;

template<int in_pin, int mode>
void robo::Interrupt<in_pin, mode>::callback()
{
    _state = !_state;
}

template<int in_pin, int mode>
void robo::Interrupt<in_pin, mode>::setup()
{
    _state = false;
    pinMode(in_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(in_pin), callback, mode);
}

template<int in_pin, int mode>
bool robo::Interrupt<in_pin, mode>::state()
{
    return _state;
}

template<int in_pin, int mode>
bool robo::Interrupt<in_pin, mode>::changed()
{
    static bool pre_state;
    bool ans = pre_state != _state;
    pre_state = _state;
    return ans;
}
