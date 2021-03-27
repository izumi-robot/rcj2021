#include <robo2019.h>

// EchoSensor(Echo-pin, Trig-pin)
robo::EchoSensor echo(2, 3);

void setup()
{
    Serial.begin(9600);
    echo.setup();
}

void loop()
{
    Serial.println(echo.read());
    delay(1000);
}