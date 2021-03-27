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

struct LCD : public LiquidCrystal_I2C
{
public:
    LCD(uint8_t addr = 0x27, uint8_t cols = 16, uint8_t rows = 2);
    void setup();
};

} // namespace robo