#include <Arduino.h>
#include "motor.h"

uint8_t robo::Motor::power_str(char *dst, uint8_t pin, int8_t power)
{
    if (dst == NULL)
        return 0;
    return sprintf_P(
        dst,
        PSTR("%1d%c%03d"),
        pin,
        power < 0 ? 'F' : 'R',
        abs(power));
}

void robo::Motor::power_str(String *dst, uint8_t pin, int8_t power)
{
    char buffer[8] = "";
    robo::Motor::power_str(buffer, pin, power);
    *dst = String(buffer);
}

String robo::Motor::power_str(uint8_t pin, int8_t power)
{
    char buffer[8] = "";
    robo::Motor::power_str(buffer, pin, power);
    return String(buffer);
}

void robo::Motor::scale_powers(int8_t (&powers)[4], int8_t max_val)
{
    int8_t p_max = 0;
    for (const int8_t &p : powers) {
        if (abs(p) > abs(p_max)) p_max = p;
    }
    float ratio = max_val / float(p_max);
    for (int8_t &p : powers) { p *= ratio; }
}

bool robo::Motor::_update(uint8_t pin, int8_t power)
{
    int8_t &dst_power = _powers[pin - 1];
    if (dst_power == power) return false;
    dst_power = power;
    return true;
}

void robo::Motor::stop()
{
    _port->print(F("1F000\n2F000\n3F000\n4F000\n"));
    memset(_powers, 0, 4);
}

int8_t robo::Motor::get_power(uint8_t pin) const { return _powers[pin - 1]; }

uint8_t robo::Motor::get_power_str(char *dst, uint8_t pin)
{
    return robo::Motor::power_str(dst, pin, get_power(pin));
}

void robo::Motor::get_power_str(String *dst, uint8_t pin)
{
    robo::Motor::power_str(dst, pin, get_power(pin));
}

String robo::Motor::get_power_str(uint8_t pin) const
{
    return robo::Motor::power_str(pin, get_power(pin));
}

void robo::Motor::set_one_motor(uint8_t pin, int8_t power)
{
    if (!_update(pin, power))
        return;
    char buffer[8] = "";
    robo::Motor::power_str(buffer, pin, power);
    _port->println(buffer);
}

void robo::Motor::set_all_motors(int8_t m1, int8_t m2, int8_t m3, int8_t m4, bool maximize)
{
    char buffer[32] = "";
    char *buf_ptr = buffer;
    int8_t ps[] = { m1, m2, m3, m4 };
    if (maximize) robo::Motor::scale_powers(ps, 100);
    for (int pin = 1; pin <= 4; pin++)
    {
        int8_t &p = ps[pin - 1];
        if (!_update(pin, p))
            continue;
        robo::Motor::power_str(buf_ptr, pin, p);
        buf_ptr[5] = '\n';
        buf_ptr += 6;
    }
    buf_ptr[0] = '\0';
    _port->print(buffer);
}

void robo::Motor::set_velocity(const float &vx, const float &vy, bool maximize)
{
    constexpr float root2 = sqrt(2.);
    int8_t e1 = int8_t((vx + vy) / root2);
    int8_t e2 = int8_t((vx - vy) / root2);
    set_all_motors(e2, e1, e1, e2, maximize);
}

void robo::Motor::set_left_right(int8_t left, int8_t right, bool maximize)
{
    set_all_motors(left, right, left, right, maximize);
}

void robo::Motor::set_dir_and_speed(const float &dir, int8_t speed, bool maximize)
{
    set_velocity(speed * cos(dir), speed * sin(dir), maximize);
}

void robo::Motor::set_rotate(bool clockwise, int8_t speed)
{
    int8_t d = clockwise ? 1 : -1;
    set_left_right(speed * d, -speed * d);
}

uint8_t robo::Motor::info(char *dst)
{
    if (dst == NULL) return 0;
    char *ptr = dst;
    for (uint8_t pin = 1; pin <= 4; pin++) {
        ptr += get_power_str(ptr, pin);
        if (pin == 4) continue;
        strcat(ptr, ", ");
        ptr += 2;
    }
    return ptr - dst;
}

String robo::Motor::info()
{
    char buff[32] = "";
    info(buff);
    return String(buff);
}