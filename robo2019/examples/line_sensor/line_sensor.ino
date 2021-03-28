#include <robo2019.h>

// LineSensor(analog-in pin)
robo::LineSensor line(3);

void setup()
{
    Serial.begin(9600);
    line.setup();
}

void loop()
{
    Serial.println(line.read());
    delay(1000);
}