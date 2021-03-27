/**
 * @file vec2d_.h
 * @brief 自作のベクトル型
 */

#pragma once

#ifdef ARDUINO

#include <ArxTypeTraits.h>
#include <ArxContainer.h>

/**
 * @namespace robo
 * @brief 自作ライブラリの機能をまとめたもの
 */
namespace robo {

/**
 * @brief 自作のベクトル型
 * @tparam T 成分の型。算術型でなければならない
 * @details
 *  実装している演算(`v`, `v1`, `v2`をベクトル値、`t`をT型の値とする):
 *  - `v1 [+,-,*,/] v2` -> `(v1.x [+,-,*,/] v2.x, v1.y [+,-,*,/] v2.y)`
 *  - `v [*,/] t` -> `(v.x [*,/] t, v.y [*,/] t)` 左右交換も可
 *  `+=`なども同様。
 *  任意のベクトルを要素数2の配列に置き換えることができる。ただし、ベクトルが絡む文脈でなければいけない。
 */
template<typename  T> class Vector2D
{
private: // static part
    static_assert(std::is_arithmetic<T>::value, "must be a number type");
public:
    /**
     * @brief 極形式から座標形式のベクトルを作成する
     * @param[in] angle 偏角。ラジアンで指定する点に注意
     * @param[in] magnitude 大きさ
     * @return 座標形式に変換したベクトル
     * @details
     *  返されるベクトルの式
     *  `(x, y)` = `(cos(angle) * magnitude, sin(angle) * magnitude)`
     */
    static Vector2D from_polar_coord(const float & angle, const T & magnitude)
    {
        robo::Vector2D<T> result;
        from_polar_coord(&result, angle, magnitude);
        return result;
    }
    /**
     * @brief 極形式から座標形式のベクトルを作成する
     * @param[in] angle 偏角。ラジアンで指定する点に注意
     * @param[in] magnitude 大きさ
     * @param[out] dst 座標形式に変換したベクトル
     * @details
     *  返されるベクトルの式
     *  `(x, y)` = `(cos(angle) * magnitude, sin(angle) * magnitude)`
     */
    static void from_polar_coord(Vector2D * dst, const float & angle, const T & magnitude)
    {
        if (dst == NULL)
            return;
        dst->x = magnitude * cos(angle);
        dst->y = magnitude * sin(angle);
    }

public: // instance properties
    //! ベクトルのx成分
    T x;
    //! ベクトルのy成分
    T y;

    /**
     * @brief デフォルトのコンストラクタ
     * @details x, yともに0で初期化される
     */
    Vector2D() : x(0), y(0) {}
    /** @brief x, y成分を指定して初期化 */
    Vector2D(const T &x, const T &y) : x(x), y(y) {}
    /** @brief コピーコンストラクター */
    Vector2D(const Vector2D &p) : x(p.x), y(p.y) {}
    /**
     * @brief 配列から初期化
     * @details 0番の要素がx、1番の要素がy
     */
    Vector2D(const T (&p)[2]);

    Vector2D& operator=(const Vector2D &rh);
    Vector2D& operator=(const T (&rh)[2]);

    inline const T& operator[](size_t index) const;
    inline T& operator[](size_t index);

    /**
     * @brief ベクトルの文字列表現を返す
     * @param[out] dst "(x, y)"
     * @return uint8_t 書き込んだ文字数
     * @note 配列の容量チェックは行っていないので注意すること。
     */
    uint8_t to_string(char *dst);
    /**
     * @brief ベクトルの文字列表現を返す
     * @param[out] dst "(x, y)"
     */
    void to_string(String *dst);
    /**
     * @brief ベクトルの文字列表現を返す
     * @return String "(x, y)"
     */
    String to_string();

    /**
     * @brief ベクトルvとの内積を返す
     * @param[in] v 内積をとるベクトル
     * @return x * v.x + y * v.y
     */
    inline T dot(const Vector2D &v) const;
    /**
     * @brief 配列tmpをベクトルとみなし、それとの内積を返す
     * @param[in] tmp 内積をとるベクトルの配列形式
     * @return x * tmp[0] + y * tmp[1]
     */
    inline T dot(const T (&tmp)[2]) const;
    /**
     * @brief ベクトル(x, y)との内積を返す
     * @param[in] x 内積をとるベクトルのx成分
     * @param[in] y 内積をとるベクトルのy成分
     * @return this->x * x + this->y * y
     */
    inline T dot(const T &x, const T &y) const;

    /**
     * @brief ベクトルvとの内積を返す
     * @param[in] v 内積をとるベクトル
     * @param[out] dst x * v.x + y * v.y
     */
    void dot(T *dst, const Vector2D &v);
    /**
     * @brief 配列tmpをベクトルとみなし、それとの内積を返す
     * @param[in] tmp 内積をとるベクトルの配列形式
     * @param[out] dst x * tmp[0] + y * tmp[1]
     */
    void dot(T *dst, const T (&tmp)[2]);
    /**
     * @brief ベクトル(x, y)との内積を返す
     * @param[in] x 内積をとるベクトルのx成分
     * @param[in] y 内積をとるベクトルのy成分
     * @param[out] dst this->x * x + this->y * y
     */
    void dot(T *dst, const T &x, const T &y);

    /**
     * @brief ベクトルの偏角を返す
     * @return 偏角(ラジアン)
     */
    inline float angle() const;
    /**
     * @brief ベクトルの偏角を返す
     * @param[out] dst 偏角(ラジアン)
     */
    void angle(float *dst);
    /**
     * @brief ベクトルの大きさを返す
     * @return ベクトルの大きさ
     */
    inline float mag() const;
    /**
     * @brief ベクトルの大きさを返す
     * @param[out] dst ベクトルの大きさ
     */
    void mag(float *dst);
};

//! Vector2D<float>のエイリアス
using V2_float = Vector2D<float>;
//! Vector2D<int>のエイリアス
using V2_int = Vector2D<int>;

} // namespace robo

template<typename T> uint8_t robo::Vector2D<T>::to_string(char * dst)
{
    if (dst == NULL) return 0;
    if (std::is_floating_point<T>::value) {
        char *ptr = dst;
        *(ptr++) = '(';
        dtostrf(x, 5, 2, ptr);
        ptr += 5;
        ptr[0] = ',';
        ptr[1] = ' ';
        ptr += 2;
        dtostrf(y, 5, 2, ptr);
        ptr += 5;
        *(ptr++) = ')';
        ptr[0] = '\0';
        return ptr - dst;
    } else if (std::is_signed<T>::value) {
        return sprintf_P(dst, PSTR("(%d, %d)"), x, y);
    } else {
        return sprintf_P(dst, PSTR("(%u, %u)"), x, y);
    }
}

#else /* ARDUINO */

#error This liblary is for Arduino.

#endif /* ARDUINO */
