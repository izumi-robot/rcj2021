/**
 * @file echo.h
 * @brief 超音波センサーHC-SR04操作用のクラス定義
 */

#pragma once

#ifndef ROBO2019_ECHO_H
#define ROBO2019_ECHO_H

#ifdef ARDUINO

#include "util.h"

/**
 * @namespace robo
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo {

/**
 * @class EchoSensor
 * @brief 超音波センサーHC-SR04操作用のクラス
 * @note robo::Sensorはutil.h内
 */
class EchoSensor : public robo::Sensor
{
public: // variables
    //! Echo pinの番号
    const int input_pin;
    //! Trig pinの番号
    const int order_pin;

public: // functions
    /**
     * @brief コンストラクタ
     * @param[in] i Echo pinの番号
     * @param[in] o Trig pinの番号
     */
    EchoSensor(int i, int o) : input_pin(i), order_pin(o) {}

    void setup() override;
    int read() override;
};

} // namespace robo

#else /* ARDUINO */

#error This liblary is for Arduino.

#endif /* ARDUINO */

#endif /* ROBO2019_ECHO_H */