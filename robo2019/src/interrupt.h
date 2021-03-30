/**
 * @file interrupt.h
 * @brief 割り込み用のクラス定義
 */

#pragma once

#ifndef ROBO2019_INTERRUPT_H
#define ROBO2019_INTERRUPT_H

#ifdef ARDUINO

#include "util.h"

// https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/

/**
 * @namespace robo
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo {

/**
 * @brief 割り込み用のテンプレートクラス(シングルトン)
 * @tparam in_pin 割り込みで監視するピン番号
 * @tparam mode 割り込みの監視モード。デフォルトはRISING
 */
template<int in_pin, int mode=RISING>
class Interrupt : public robo::SingletonBase<Interrupt<in_pin, mode>>
{
private:
    /**
     * @brief 状態記憶用の変数
     * @details 割り込みが発生するごとにtrue/falseが切り替わる
     */
    static volatile bool _state;

    /**
     * @fn void callback()
     * @brief 割り込み発生時に呼び出される
     * @details _stateを切り替える
     */
    static void callback();

public:
    /**
     * @fn void setup();
     * @brief 割り込みのセットアップを行う
     * @note 全体のsetup内で呼ばないと他の機能が使えない
     */
    void setup();

    /**
     * @fn bool state()
     * @brief 現在保存されている状態を返す
     * @return _stateの値
     */
    bool state();

    /**
     * @fn bool changed()
     * @brief 最後の呼び出しから_stateが変化したかどうか
     * @return 変化していたらtrue
     */
    bool changed();
};

} // namespace robo

#else /* ARDUINO */

#error This liblary is for Arduino.

#endif /* ARDUINO */

#endif /* ROBO2019_INTERRUPT_H */