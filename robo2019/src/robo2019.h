/**
 * @file robo2019.h
 * @brief 自作ライブラリのエントリーポイント
 */

#pragma once

#ifndef ROBO2019_H
#define ROBO2019_H

#ifdef ARDUINO

#include "bno055.h"
#include "echo.h"
#include "interrupt.h"
#include "lcd.h"
#include "line_sensor.h"
#include "motor.h"
#include "move_info.h"
#include "openmv.h"
#include "util.h"
#include "vec2d.h"

#else /* ARDUINO */

#error This liblary is for Arduino.

#endif /* ARDUINO */

#endif /* ROBO2019_H */