/**
 * @file motor2.h
 * @brief Daisen MCB操作用のクラス定義
 */

#pragma once

#ifdef ARDUINO

#include <Print.h>
#include "vec2d.h"

/**
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo
{

/**
 * @brief MCB操作用のクラス
 * @details モーターの配置についてはREADMEを参照
 */
class Motor
{
public: // static functions
    /**
     * @brief パワー設定用の文字列を取得する
     * @param[out] dst パワー設定用の文字列
     * @param[in] pin モーターのピン番号
     * @param[in] power モーターのパワー
     * @return uint8_t 書き込んだ文字数
     * @note dstには少なくとも6文字を格納する容量が必要。容量チェックは行っていないため、注意すること。
     */
    static uint8_t power_str(char *dst, uint8_t pin, int8_t power);

    /**
     * @brief パワー設定用の文字列を取得する
     * @param[out] dst パワー設定用の文字列
     * @param[in] pin モーターのピン番号
     * @param[in] power モーターのパワー
     */
    static void power_str(String *dst, uint8_t pin, int8_t power);

    /**
     * @brief パワー設定用の文字列を取得する
     * @param[in] pin モーターのピン番号
     * @param[in] power モーターのパワー
     * @return パワー設定用の文字列
     */
    static String power_str(uint8_t pin, int8_t power);

    /**
     * @brief モーターのパワーの配列を拡大/縮小する
     * @param[in,out] powers 対象
     * @param max_val パワーの最大値
     * @example
     *  ```C++
     *  int8_t powers[] = { 67, 0, 50, -80 };
     *  scale_powers(powers, 100);
     *  // powers => { 83, 0, 62, -100 }
     *  ```
     */
    static void scale_powers(int8_t (&powers)[4], int8_t max_val);

private: // variables
    //! モーターのパワー
    int8_t _powers[4];
    //! MCBがつながっているシリアルポート
    Print *_port;

private:
    /**
     * @brief パワー更新用
     * @param[in] pin モーターのピン番号
     * @param[in] power モーターのパワー
     * @return true=>MCBに文字を流すべき
     */
    bool _update(uint8_t pin, int8_t power);

public:
    /**
     * @brief Construct a new Motor object
     * @note シリアルポートがSerialであるものとして初期化
     */
    Motor() : _powers{0, 0, 0, 0}, _port(NULL) {
        _port = &Serial;
    }
    /**
     * @brief Construct a new Motor object
     * @param serial MCBがつながっているシリアルポート
     */
    Motor(Print *port) : _powers{0, 0, 0, 0}, _port(port) {}

    /** @brief 停止させる */
    void stop();

    /**
     * @brief モーターのパワーを取得する
     * @param[in] pin モーターのピン番号
     * @return int8_t モーターのパワー
     */
    inline int8_t get_power(uint8_t pin) const;

    /**
     * @brief モーターのパワーの文字列表示を取得する
     * @param[out] dst 結果の文字列を保存するポインター
     * @param[in] pin モーターのピン番号
     */
    inline uint8_t get_power_str(char *dst, uint8_t pin);

    /**
     * @brief モーターのパワーの文字列表示を取得する
     * @param[out] dst 結果の文字列の保存先
     * @param[in] pin モーターのピン番号
     */
    inline void get_power_str(String *dst, uint8_t pin);

    /**
     * @brief モーターのパワーの文字列表示を取得する
     * @param[in] pin モーターのピン番号
     * @return String 結果の文字列
     */
    inline String get_power_str(uint8_t pin) const;

    /**
     * @brief モーター1つのパワーを設定する
     * @param[in] pin 設定するモーターのピン番号
     * @param[in] power 設定するモーターのパワー
     */
    void set_one_motor(uint8_t pin, int8_t power);

    /**
     * @brief モーター全てのパワーを設定する
     * @param[in] m1 ピン番号1のモーターに設定するパワー
     * @param[in] m2 ピン番号2のモーターに設定するパワー
     * @param[in] m3 ピン番号3のモーターに設定するパワー
     * @param[in] m4 ピン番号4のモーターに設定するパワー
     * @param[in] maximize パワーを最大化するかどうか(デフォルトはfalse)
     */
    void set_all_motors(int8_t a, int8_t b, int8_t c, int8_t d, bool maximize = false);

    /**
     * @brief 機体が平行移動移動するように速度ベクトルを設定する
     * @param[in] vx ベクトルのx成分
     * @param[in] vy ベクトルのy成分
     * @param[in] maximize パワーを最大化するかどうか(デフォルトはfalse)
     * @details 座標系の定義はREADMEを参照
     */
    void set_velocity(const float &vx, const float &vy, bool maximize = false);

    /**
     * @brief 機体が平行移動移動するように速度ベクトルを設定する
     * @param[in] vel 設定するベクトル
     * @param[in] maximize パワーを最大化するかどうか(デフォルトはfalse)
     * @details 座標系の定義はREADMEを参照
     */
    inline void set_velocity(const robo::V2_float &vel, bool maximize = false);

    /**
     * @brief 左輪と右輪でわけてパワーを設定する
     * @param[in] left 左輪のパワー
     * @param[in] right 右輪のパワー
     * @param[in] maximize パワーを最大化するかどうか(デフォルトはfalse)
     * @details 左輪、右輪の定義はREADMEを参照
     */
    inline void set_left_right(int8_t left, int8_t right, bool maximize = false);

    /**
     * @brief 方向と速さで機体の平行移動のベクトルを設定する
     * @param[in] dir ベクトルの方向(ラジアン)
     * @param[in] speed 速度
     * @param[in] maximize パワーを最大化するかどうか(デフォルトはfalse)
     * @details 方向についてはREADMEを参照。maxmize=trueの場合、speedは無視される。
     */
    inline void set_dir_and_speed(const float &dir, int8_t speed, bool maximize = false);

    /**
     * @brief 機体が回転するようにパワーを設定する
     * @param[in] clockwise 回転の方向(時計回りかどうか)
     * @param[in] speed 回転速度
     */
    void set_rotate(bool clockwise, int8_t speed);

    /**
     * @brief 現在のパワーを見やすい文字列で出力する
     * @param[out] dst 文字列を書き込む先
     * @return uint8_t 書き込んだ文字数
     * @note バッファオーバーランの可能性あり。少なくとも27文字分の容量が必要
     */
    uint8_t info(char *dst);

    /**
     * @brief 現在のパワーを見やすい文字列で取得する
     * @return String 結果の文字列
     */
    String info();
};

} // namespace robo

inline void robo::Motor::set_velocity(const robo::V2_float &vel, bool maximize)
{
    set_velocity(vel.x, vel.y, maximize);
}

#else /* ARDUINO */

#error This liblary is for Arduino.

#endif  /* ARDUINO */