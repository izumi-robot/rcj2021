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

    //! カメラの座標系で中心の位置
    const Position center{90, 70};

    /**
     * @brief カメラが読み取った情報を表現するクラス
     */
    class Frame {
    public:
        /**
         * @brief ボールの座標
         * @details ボールがなかった場合はNULL。
         * @note このクラスがポインタが指す先のデータの所有権を持つ。すなわち、Frameのデストラクタ呼び出し時に自動的にdeleteされる。
         */
        Position *ball_pos;
        /**
         * @brief 黄色のゴールの座標
         * @details ゴールがなかった場合はNULL。
         * @note このクラスがポインタが指す先のデータの所有権を持つ。すなわち、Frameのデストラクタ呼び出し時に自動的にdeleteされる。
         */
        Position *y_goal_pos;
        /**
         * @brief 青色のゴールの座標
         * @details ゴールがなかった場合はNULL。
         * @note このクラスがポインタが指す先のデータの所有権を持つ。すなわち、Frameのデストラクタ呼び出し時に自動的にdeleteされる。
         */
        Position *b_goal_pos;

        /**
         * @brief Construct a new Frame object
         * @param ball_x ボールのx座標
         * @param ball_y ボールのy座標
         * @param y_goal_x 黄色のゴールのx座標
         * @param y_goal_y 黄色のゴールのy座標
         * @param b_goal_x 青色のゴールのx座標
         * @param b_goal_y 青色のゴールのy座標
         */
        Frame(
            uint16_t ball_x, uint16_t ball_y,
            uint16_t y_goal_x, uint16_t y_goal_y,
            uint16_t b_goal_x, uint16_t b_goal_y
        );

        /**
         * @brief Construct a new Frame object
         * @param ball_pos ボールの座標データへのポインタ
         * @param y_goal_pos 黄色のゴールの座標データへのポインタ
         * @param b_goal_pos 青色のゴールの座標データへのポインタ
         */
        Frame(Position *ball_pos, Position *y_goal_pos, Position *b_goal_pos);

        /**
         * @brief Destroy the Frame object
         * @note ball_pos, y_goal_pos, b_goal_posがdeleteされる
         */
        ~Frame();

        /**
         * @brief Frameの文字列表現を取得
         * @param[out] dst 文字列を書き込む先
         * @return uint8_t 書き込んだ文字数
         * @note バッファオーバーランを起こす可能性があるので注意。容量は最大で70文字程度食う。
         */
        uint8_t to_string(char *dst);
    };

    /**
     * @brief OpenMVが送る情報をI2C通信で読み取るクラス
     */
    class Reader {
    private: // variables
        //! 通信で使うI2Cバス
        TwoWire &_wire;

    public:
        //! OpenMVのI2Cアドレス
        const uint8_t address;

    public: // functions
        /**
         * @brief Construct a new Reader object
         * @param addr OpenMVのアドレス
         * @param wire 通信で使うI2Cバス
         */
        Reader(uint8_t addr, TwoWire &wire = Wire);

    private:
        /**
         * @brief データを捨てる
         * @param size 捨てるデータサイズ(バイト)
         */
        void pass_data(uint8_t size);

        /**
         * @brief 2バイト読んで1つの値にする
         * @return uint16_t 読んだ値
         * @details 先に読み込んだ1バイトが下8ビット、次に読み込んだ1バイトが上8ビットになる
         */
        uint16_t read_2byte();

        /**
         * @brief 座標のデータ1つを読み込む
         * @return Position* 読み込んだデータのポインタ
         * @details オブジェクトが見つからなかった場合に送られるデータをOpenMVから受け取った場合、NULLを返す
         */
        Position* read_pos();

    public:
        /**
         * @brief I2Cをセットアップする
         */
        void setup();

        /**
         * @brief Frameを読み込む
         * @return Frame* 読み込んだFrameのポインタ
         * @note 読み込みに失敗した、またはオブジェクトが一つもなかった場合はNULL
         */
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