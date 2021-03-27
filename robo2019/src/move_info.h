/**
 * @file move_info.h
 * @author Kawamoto Hironori
 * @brief モーターの移動情報を表現するクラス定義
 */

#pragma once

#include "vec2d.h"
#include "motor.h"

/**
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo
{

/**
 * @brief モーターの移動情報を表現するクラス定義
 */
namespace move_info
{

    class MoveInfo
    {
    public:
        virtual void apply(robo::Motor &motor) = 0;
        virtual uint8_t to_string(char *dst) = 0;
        virtual String to_string() = 0;
    };

    class Stop final : public MoveInfo
    {
    public:
        void apply(robo::Motor &motor) override;
        uint8_t to_string(char *dst) override;
        String to_string() override;
    };

    class Translate final : public MoveInfo
    {
    private:
        robo::V2_float vec;
        bool maximize;

    public:
        Translate(const float & vx, const float & vy, bool maximize = false);
        Translate(const robo::V2_float &vec, bool maximize = false);

        void apply(robo::Motor &motor) override;
        uint8_t to_string(char *dst) override;
        String to_string() override;
    };

    class Rotate final : public MoveInfo
    {
    private:
        const bool clockwise;
        const int8_t speed;

    public:
        Rotate(const bool clockwise, const int8_t speed);

        void apply(robo::Motor &motor) override;
        uint8_t to_string(char *dst) override;
        String to_string() override;
    };

} // namespace move_info

} // namespace robo

#ifndef ARDUINO

#error This liblary is for Arduino.

#endif /* ARDUINO */