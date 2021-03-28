/**
 * @file lcd.h
 * @brief LiquidCrystalDisplayラッパー
 */

#pragma once

#include <LiquidCrystal_I2C.h>

/**
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo
{

/**
 * @brief LiquidCrystal_I2Cのラッパ
 */
struct LCD : public LiquidCrystal_I2C
{
public:
    /**
     * @brief Construct a new LCD object
     * @param addr LCDのI2Cアドレス。デフォルトは0x27
     * @param cols LCDの列数(1行の文字数)。デフォルトは16
     * @param rows LCDの行数。デフォルトは2
     */
    LCD(uint8_t addr = 0x27, uint8_t cols = 16, uint8_t rows = 2);

    /**
     * @brief 全体のセットアップ内で呼び出すと便利な関数
     */
    void setup();
};

} // namespace robo