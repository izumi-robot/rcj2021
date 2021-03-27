/**
 * @file sensors.h
 * @brief センサー統合用のクラス定義
 */

#pragma once

#ifndef ROBO2019_SENSORS_H
#define ROBO2019_SENSORS_H

#ifdef ARDUINO

#include "util.h"

/**
 * @namespace robo
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo
{

using robo::Sensor;

/**
 * @class Sensors
 * @brief センサー統合用のクラス
 */
class Sensors
{
public:
    /**
     * @class Values
     * @brief 値保存用のクラス
     */
    class Values
    {
    private:
        //! 情報取得元のインスタンス
        Sensors *ptr;
        //! 値を保存する配列
        int _list[3];

    public:
        Values() = delete;
        /**
         * @brief 引数のポインタで初期化
         * @param[in] p 情報取得元のポインタ
         */
        Values(Sensors *p) : ptr(p), _list{ 0, 0, 0 } {}

        /**
         * @fn void update()
         * @brief 情報を更新
         */
        void update()
        {
            for (int i = 0; i < 3; ++i)
            {
                _list[i] = (*ptr)[i].read();
            }
        }

        //!
        inline const int &left()  const { return _list[0]; }
        inline const int &right() const { return _list[1]; }
        inline const int &back()  const { return _list[2]; }
        inline const int &operator[](size_t i) const { return _list[i]; }
    };

private:
    Sensor _list[3];
    Sensor &_left, &_right, &_back;
    Values _values;

public:
    Sensor() = delete;
    /**
     * @brief センサー3つをまとめる
     * @param[in] left  機体の左側にあるセンサー
     * @param[in] right 機体の右側にあるセンサー
     * @param[in] back  機体の後ろ側にあるセンサー
     */
    Sensor(const Sensor &left, const Sensor &right, const Sensor &back)
        : _list{ left, right, back }, _values(this) {}

    inline const Sensor &left()  const { return _list[0]; }
    inline const Sensor &right() const { return _list[1]; }
    inline const Sensor &back()  const { return _list[2]; }

    inline Sensor &left()  { return _list[0]; }
    inline Sensor &right() { return _list[1]; }
    inline Sensor &back()  { return _list[2]; }

    inline const Sensor &operator[](size_t i) const { return _list[i]; }
    inline Sensor &operator[](size_t i) { return _list[i]; }

    inline const Values &values() const { return _values; }
    inline Values &values() { return _values; }

    void setup()
    {
        for (int i = 0; i < 3; ++i)
        {
            _list[i].setup();
        }
        update();
    }

    inline void update()
    {
        _values.update();
    }
};

} // namespace robo

#else /* ARDUINO */

#error This liblary is for Arduino.

#endif /* ARDUINO */

#endif /* ROBO2019_SENSORS_H */