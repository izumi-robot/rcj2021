/**
 * @file util.h
 * @brief ユーティリティ
 */

#pragma once

#ifndef ROBO2019_UTIL_H
#define ROBO2019_UTIL_H

#ifdef ARDUINO

/**
 * @namespace robo
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo
{

/**
 * @brief センサー類の基底クラス
 */
class Sensor
{
public:
    /**
     * @fn void setup();
     * @brief モーターのセットアップを行う
     * @note 全体のsetup内で呼ばないと他の機能が使えない。
     */
    virtual void setup() = 0;
    /**
     * @fn unsigned int read()
     * @brief センサーの値を読む
     * @return 読んだ値
     */
    virtual int read() = 0;
};

/**
 * @class SingletonBase
 * @brief シングルトンのテンプレートクラス
 * @tparam Derived 派生のシングルトンクラス
 * @details https://qiita.com/_meki/items/96acf0b45c75c17d6dec
 * @note
        シングルトンクラスFooを作る例:
        class Foo : SingletonBase<Foo> { ... };
 */
template<class Derived> class SingletonBase
{
    friend Derived;

protected:
    SingletonBase() = default;
    ~SingletonBase() = default;

    SingletonBase(const SingletonBase &) = delete;
    SingletonBase& operator=(const SingletonBase &) = delete;

public:
    /**
     * @fn Derived& instance()
     * @brief シングルトンのインスタンス取得用
     * @return インスタンスの参照
     */
    static Derived& instance();
};

} // namespace robo

template<typename Derived> Derived& robo::SingletonBase<Derived>::instance()
{
    static Derived ins;
    return ins;
}

#else /* ARDUINO */

#error This liblary is for Arduino.

#endif /* ARDUINO */

#endif /* ROBO2019_UTIL_H */