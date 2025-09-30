#pragma once
#include <algorithm>
#include <random>
#include "vec2.h"
#include "window.h"

// std::mt19937 rng{std::random_device{}()};

struct Ball
{
    Vec2<double> point;
    Vec2<double> direction;
    static constexpr double SPEED_INCREASE_FACTOR = 1.2;
    static constexpr double ABS_MAX_SPEED = static_cast<double>(WindowSpecification::WIDTH >> 3);
    // static constexpr double PI = atan(1) * 4;
    // static constexpr double ABS_MAX_SPEED = -std::pi
    
    bool checkVerticalCollision() {
        return (
            point.y <= 0 ||
            point.y >= WindowSpecification::HEIGHT
        );
    }

    void update(double& elapsed) {        
        point += direction * elapsed;

        if (checkVerticalCollision())
            direction.reflect_y();
    }

    void bounce_horizontal() {
        direction.reflect_x();
        increase_speed();
    }

    void bounce_vertical() {
        direction.reflect_y();
    }
    
    void increase_speed() {
        direction.x = std::clamp(direction.x * SPEED_INCREASE_FACTOR, -ABS_MAX_SPEED, ABS_MAX_SPEED);
        direction.y = std::clamp(direction.y * SPEED_INCREASE_FACTOR, -ABS_MAX_SPEED, ABS_MAX_SPEED);
    }

    void reset() {
        // move to middle
        point.x = WindowSpecification::WIDTH >> 1;
        point.y = WindowSpecification::HEIGHT >> 1;

        // reset direction
        direction.x = (direction.x > 0 ? -1.0 : 1.0);
        direction.y = 0;
    }
};