#include <Arduino.h>
#include "vec2d.h"

template <typename T>
robo::Vector2D<T>::Vector2D(const T (&p)[2]) : x(p[0]), y(p[1]) {}

template <typename T>
robo::Vector2D<T> &robo::Vector2D<T>::operator=(const robo::Vector2D<T> &tmp)
{
    x = tmp.x;
    y = tmp.y;
    return *this;
}

template <typename T>
robo::Vector2D<T> &robo::Vector2D<T>::operator=(const T (&tmp)[2])
{
    x = tmp[0];
    y = tmp[1];
    return *this;
}

#define SUBSCRIBE_IMPL(_type_, _spec_) \
    template <typename T>              \
    _type_ robo::Vector2D<T>::operator[](size_t index) _spec_ { return index ? x : y; }

SUBSCRIBE_IMPL(const T &, const)
SUBSCRIBE_IMPL(T &, )

#undef SUBSCRIBE_IMPL

template <typename T>
void robo::Vector2D<T>::to_string(String *dst)
{
    if (dst == NULL)
        return;
    char buffer[64] = "";
    to_string(buffer);
    *dst = buffer;
}
template <typename T>
String robo::Vector2D<T>::to_string()
{
    char buffer[64] = "";
    to_string(buffer);
    return String(buffer);
}

template <typename T>
T robo::Vector2D<T>::dot(const robo::Vector2D<T> &v) const { return x * v.x + y * v.y; }
template <typename T>
T robo::Vector2D<T>::dot(const T (&tmp)[2]) const { return x * tmp[0] + y * tmp[1]; }
template <typename T>
T robo::Vector2D<T>::dot(const T &x, const T &y) const { return this->x * x + this->y * y; }

template <typename T>
void robo::Vector2D<T>::dot(T *dst, const robo::Vector2D<T> &v)
{
    if (dst == NULL)
        return;
    *dst = x * v.x + y * v.y;
}
template <typename T>
void robo::Vector2D<T>::dot(T *dst, const T (&tmp)[2])
{
    if (dst == NULL)
        return;
    *dst = x * tmp[0] + y * tmp[1];
}
template <typename T>
void robo::Vector2D<T>::dot(T *dst, const T &x, const T &y)
{
    if (dst == NULL)
        return;
    *dst = this->x * x + this->y * y;
}

template <typename T>
float robo::Vector2D<T>::angle() const
{
    return atan2(y, x);
}
template <typename T>
void robo::Vector2D<T>::angle(float *dst)
{
    if (dst == NULL)
        return;
    *dst = angle();
}

template <typename T>
float robo::Vector2D<T>::mag() const
{
    return sqrt(x * x + y * y);
}
template <typename T>
void robo::Vector2D<T>::mag(float *dst)
{
    if (dst == NULL)
        return;
    *dst = mag();
}

#define IM4(_mc_) _mc_(+) _mc_(-) _mc_(*) _mc_(/)
#define IM2(_mc_) _mc_(*) _mc_(/)

// operator(Vec2D, Vec2D)
#define OP_IMPL(_op_)                                                                                \
    template <typename T>                                                                            \
    robo::Vector2D<T> operator _op_(const robo::Vector2D<T> &lh, const robo::Vector2D<T> &rh) \
    {                                                                                                \
        return robo::Vector2D<T>(lh.x _op_ rh.x, lh.y _op_ rh.y);                                    \
    }

IM4(OP_IMPL)

#undef OP_IMPL

#define IOP_IMPL(_op_)                                                                      \
    template <typename T>                                                                   \
    robo::Vector2D<T> &operator _op_##=(robo::Vector2D<T> &lh, const robo::Vector2D<T> &rh) \
    {                                                                                       \
        lh.x _op_## = rh.x;                                                                 \
        lh.y _op_## = rh.y;                                                                 \
        return lh;                                                                          \
    }

IM4(IOP_IMPL)

#undef IOP_IMPL

// operator(Vec2D, T (&)[2])
#define OP_IMPL(_op_)                                                                    \
    template <typename T>                                                                \
    robo::Vector2D<T> operator _op_(const robo::Vector2D<T> &lh, const T(&rh)[2]) \
    {                                                                                    \
        return robo::Vector2D<T>(lh.x _op_ rh[0], lh.y _op_ rh[1]);                      \
    }

IM4(OP_IMPL)

#undef OP_IMPL

#define IOP_IMPL(_op_)                                                          \
    template <typename T>                                                       \
    robo::Vector2D<T> &operator _op_##=(robo::Vector2D<T> &lh, const T(&rh)[2]) \
    {                                                                           \
        lh.x _op_## = rh[0];                                                    \
        lh.y _op_## = rh[1];                                                    \
        return lh;                                                              \
    }

IM4(IOP_IMPL)

#undef IOP_IMPL

// operator(T (&)[2], Vec2D)
#define OP_IMPL(_op_)                                                                    \
    template <typename T>                                                                \
    robo::Vector2D<T> operator _op_(const T(&lh)[2], const robo::Vector2D<T> &rh) \
    {                                                                                    \
        return robo::Vector2D<T>(lh[0] _op_ rh.x, lh[1] _op_ rh.y);                      \
    }

IM4(OP_IMPL)

#undef OP_IMPL

// operator(Vec2D, T)
#define OP_IMPL(_op_)                                                                \
    template <typename T>                                                            \
    robo::Vector2D<T> operator _op_(const robo::Vector2D<T> &lh, const T &rh) \
    {                                                                                \
        return robo::Vector2D<T>(lh.x _op_ rh, lh.y _op_ rh);                        \
    }

IM2(OP_IMPL)

#undef OP_IMPL

#define IOP_IMPL(_op_)                                                      \
    template <typename T>                                                   \
    robo::Vector2D<T> &operator _op_##=(robo::Vector2D<T> &lh, const T &rh) \
    {                                                                       \
        lh.x _op_## = rh;                                                   \
        lh.y _op_## = rh;                                                   \
        return lh;                                                          \
    }

IM2(IOP_IMPL)

#undef IOP_IMPL

// operator(T, Vec2D)
#define OP_IMPL(_op_)                                                                \
    template <typename T>                                                            \
    robo::Vector2D<T> operator _op_(const T &lh, const robo::Vector2D<T> &rh) \
    {                                                                                \
        return robo::Vector2D<T>(lh _op_ rh.x, lh _op_ rh.y);                        \
    }

IM2(OP_IMPL)

#undef OP_IMPL

#undef IM4
#undef IM2

template <typename T>
bool operator==(const robo::Vector2D<T> &lh, const robo::Vector2D<T> &rh)
{
    return lh.x == rh.x && lh.y == rh.y;
}
template <typename T>
bool operator!=(const robo::Vector2D<T> &lh, const robo::Vector2D<T> &rh) { return lh.x != rh.x || lh.y != rh.y; }
