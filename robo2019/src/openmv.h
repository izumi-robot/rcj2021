/**
 * @file openmv.h
 * @brief I2C通信の読み取り用のクラス定義
 */

#pragma once

#ifdef ARDUINO

#include <Wire.h>

#include "vec2d.h"

/**
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo
{

/** @brief OpenMV関連の機能をまとめたもの */
namespace openmv {
    /** @brief ボールのカメラ視点の座標を表すエイリアス */
    using Position = robo::Vector2D<uint16_t>;

    const Position center{90, 70};

    class Frame {
    public:
        using PosPtr = Position *;
        Position *ball_pos;
        Position *y_goal_pos;
        Position *b_goal_pos;

        Frame(
            uint16_t ball_x, uint16_t ball_y,
            uint16_t y_goal_x, uint16_t y_goal_y,
            uint16_t b_goal_x, uint16_t b_goal_y
        );

        Frame(Position *ball_pos, Position *y_goal_pos, Position *b_goal_pos);
        ~Frame();
        uint8_t to_string(char *dst);
    };

    class Reader {
    private: // variables
        TwoWire &_wire;

    public:
        const uint8_t address;

    public: // functions
        Reader(uint8_t addr, TwoWire &wire = Wire);

    private:
        void pass_data(uint8_t size);
        uint16_t read_2byte();
        Position* read_pos();

    public:
        void setup();
        Frame* read_frame();
    };

    constexpr float pos2dir(const Position & pos)
    {
        return atan2(
            -float(pos.x) + 90, // = -(pos.x - 90)
            float(pos.y) - 70
        );
    }
} // namespace openmv

} // namespace robo

#else /* ARDUINO */

#error This liblary is for Arduino.

#endif /* ARDUINO */