#include <robo2019.h>
#include <SoftwareSerial.h>

// for mega

SoftwareSerial motor_serial(12, 13);
robo::Motor motor(&motor_serial);
constexpr int power = 60;
using namespace robo::move_info;
MoveInfo * infos[] = {
    new Translate(100, 0),
    new Rotate(true, power),
    new Translate(0, -power),
    new Translate(robo::V2_float::from_polar_coord(PI / 4, power)),
    new Stop()
};

void setup()
{
    motor_serial.begin(19200);
    Serial.begin(9600);
}

void loop()
{
    #define LOG Serial.println(motor.info());
    infos[0]->apply(motor);
    for (uint8_t i = 0; i < 5; i++) {delay(500);infos[i]->apply(motor);LOG}
    delay(5000);
}
