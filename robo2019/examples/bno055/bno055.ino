#include <robo2019.h>

//  Adafruit_BNO055(id, addr)
robo::BNO055 bno055( 0, 0x28);

void setup()
{
    Serial.begin(9600);
    Serial.println("start");
    bno055.setup();
}

void loop()
{
    if (!bno055.detected()) Serial.println("bno not detected");
    // 最初の方向からの差分を取得
    // 反時計回りが正
    // ラジアン(-PI以上PI以下)
    float dir = bno055.get_geomag_direction();
    // 度数法(-180以上180以下)で表示
    Serial.println(dir * 180 / PI);
    delay(1000);
}