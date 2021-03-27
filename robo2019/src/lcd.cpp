#include <Arduino.h>
#include "lcd.h"

robo::LCD::LCD(uint8_t addr, uint8_t cols, uint8_t rows)
: LiquidCrystal_I2C(addr, cols, rows) {}

void robo::LCD::setup()
{
    LiquidCrystal_I2C::init();
    LiquidCrystal_I2C::backlight();
    LiquidCrystal_I2C::setCursor(0, 0);
}