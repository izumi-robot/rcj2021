/**
 * @file line_sensor.h
 * @brief ラインセンサー操作用のクラス定義
 */

#pragma once

#ifndef ROBO2019_LINE_SENSOR_H
#define ROBO2019_LINE_SENSOR_H

#ifdef ARDUINO

#include "util.h"

/**
 * @namespace robo
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo {

/**
 * @class LineSensor
 * @brief ラインセンサー操作用のクラス
 * @note robo::Sensorはutil.h内
 */
class LineSensor : public robo::Sensor
{
public:
    //! センサーの値がこれ以上であれば白
    static int white_border;

    //! センサーのピン
    const uint8_t in_pin;

    /**
     * @fb bool iswhite(int c)
     * @brief 値が白色か判別する
     * @param[in] c 検査する値
     * @return 白色かどうか
     */
    static bool iswhite(int c);

    LineSensor() = delete;
    /**
     * @brief コンストラクタ
     * @param[in] i ラインセンサーのピン番号
     */
    LineSensor(uint8_t i) : in_pin(i) {}

    inline void setup() override;
    inline int read() override;
};

int LineSensor::white_border = 600;

bool LineSensor::iswhite(int c)
{
    return c >= white_border;
}

void LineSensor::setup()
{
    pinMode(in_pin, INPUT);
}

int LineSensor::read()
{
    return analogRead(in_pin);
}

} // namespace robo

#else /* ARDUINO */

#error This liblary is for Arduino.

#endif /* ARDUINO */

#endif /* ROBO2019_LINE_SENSOR_H */