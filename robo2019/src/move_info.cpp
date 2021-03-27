#include <Arduino.h>
#include "move_info.h"

//implementations of robo::move_info::Stop
void robo::move_info::Stop::apply(robo::Motor & motor)
{
    motor.stop();
}

uint8_t robo::move_info::Stop::to_string(char * dst)
{
    if (dst == NULL) return 0;
    strcat_P(dst, PSTR("MoveInfo: Stop"));
    return 14;
}

String robo::move_info::Stop::to_string()
{
    char buffer[16] = "";
    to_string(buffer);
    return String(buffer);
}

//implementations of robo::move_info::Translate
robo::move_info::Translate::Translate(const float & vx, const float & vy, bool maximize)
: vec(vx, vy), maximize(maximize) {}

robo::move_info::Translate::Translate(const robo::V2_float & vec, bool maximize)
: vec(vec), maximize(maximize) {}

void robo::move_info::Translate::apply(robo::Motor & motor)
{
    motor.set_velocity(vec, maximize);
}

uint8_t robo::move_info::Translate::to_string(char * dst)
{
    if (dst == NULL) return 0;
    char * ptr = dst;
    strcat_P(ptr, PSTR("MoveInfo: Translate("));
    ptr += 20; // len("MoveInfo: Translate(") == 20
    ptr += vec.to_string(ptr);
    ptr += sprintf_P(ptr, PSTR(", %s)"), maximize ? PSTR("true") : PSTR("false"));
    return ptr - dst;
}

String robo::move_info::Translate::to_string()
{
    char buffer[64] = "";
    to_string(buffer);
    return String(buffer);
}

//implementations of robo::move_info::Rotate
robo::move_info::Rotate::Rotate(bool clockwise, int8_t speed)
: clockwise(clockwise), speed(speed) {}

void robo::move_info::Rotate::apply(robo::Motor & motor)
{
    motor.set_rotate(clockwise, speed);
}

uint8_t robo::move_info::Rotate::to_string(char * dst)
{
    if (dst == NULL) return 0;
    return sprintf_P(
        dst,
        PSTR("MoveInfo: Rotate(%s, %d)"),
        clockwise ? PSTR("true") : PSTR("false"),
        speed
    );
}

String robo::move_info::Rotate::to_string()
{
    char buffer[64] = "";
    to_string(buffer);
    return String(buffer);
}
