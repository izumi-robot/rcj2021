/**
 * @file bno055.h
 * @brief BNO055センサー操作用のクラス定義
 */

#pragma once

#ifndef ROBO2019_BNO055_H
#define ROBO2019_BNO055_H

#ifdef ARDUINO

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "util.h"

/**
 * @namespace robo
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo {

/**
 * @class BNO055
 * @brief Adafruit_BNO055の子クラス
 */
class BNO055 final : public virtual Adafruit_BNO055
{
private:
    //! bnoを検知したかどうか
    bool _detected = false;
    //! 最新の、ジャイロセンサーで算出した方向
    float _last_gyro_dir = 0;
    /**
     * @brief 地磁気のズレ
     * @details `0`が指す向きの、最初の向きとのズレ
     */
    float _geomag_diff = 0;

public:
    using Adafruit_BNO055::Adafruit_BNO055;

    /**
     * @brief bno055のセットアップを行う
     * @note 全体のsetup内で呼ばないと他の機能が使えない
     */
    void setup();
    /**
     * @brief 現在向いている方向をラジアンで取得
     * @return 現在向いている方向
     * @note ラジアンの値は、0を最初の向きとして、そこから正回転が反時計回り
     */
    float get_geomag_direction();
    /**
     * @brief 現在向いている方向をラジアンで取得
     * @param{out} dst 現在向いている方向
     * @note ラジアンの値は、0を最初の向きとして、そこから正回転が反時計回り
     */
    void get_geomag_direction(float *dst);

    /**
     * @fn bool detected()
     * @brief bnoが検知されたかどうか
     * @return 検知されたらtrue
     */
    bool detected();
};

} // namespace robo

#else /* ARDUINO */

#error This liblary is for Arduino.

#endif /* ARDUINO */

#endif /* ROBO2019_BNO055_H */