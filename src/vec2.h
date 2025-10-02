#pragma once

#include <cmath>

// slight alteration of https://cp-algorithms.com/geometry/basic-geometry.html
template <typename T>
struct Vec2 {
    T x, y;
    Vec2() {}
    Vec2(T x, T y): x(x), y(y) {}
    Vec2& reflect_x() {
        x *= -1;
        return *this;
    }
    Vec2& reflect_y() {
        y *= -1;
        return *this;
    }
    Vec2& operator+=(const Vec2 &t) {
        x += t.x;
        y += t.y;
        return *this;
    }
    Vec2& operator-=(const Vec2 &t) {
        x -= t.x;
        y -= t.y;
        return *this;
    }
    Vec2& operator*=(T t) {
        x *= t;
        y *= t;
        return *this;
    }
    Vec2& operator/=(T t) {
        x /= t;
        y /= t;
        return *this;
    }
    Vec2 operator+(const Vec2 &t) const {
        return Vec2(*this) += t;
    }
    Vec2 operator-(const Vec2 &t) const {
        return Vec2(*this) -= t;
    }
    Vec2 operator*(T t) const {
        return Vec2(*this) *= t;
    }
    Vec2 operator/(T t) const {
        return Vec2(*this) /= t;
    }
    double calculateMagnitude() const {
        return std::sqrt(pow(x, 2) + pow(y, 2));
    }
};

template <typename T>
inline Vec2<T> operator*(T a, Vec2<T> b) {
    return b * a;
}