#pragma once
#include <algorithm>
#include <random>
#include <cmath>

#include "vec2.h"
#include "window.h"
#include "constants.h"


struct Ball
{
    Vec2<double> point;
    Vec2<double> direction;
    static constexpr double SPEED_INCREASE_FACTOR = 1.2;
    static constexpr double ABS_MAX_SPEED = static_cast<double>(WindowSpecification::WIDTH >> 3);
    static constexpr double ABS_MIN_SPEED = 0.70710678118; // sqrt(0.5^2 + 0.5^2)
    static constexpr double ABS_MAX_DIR_Y = 0.5;
    static constexpr double ABS_MIN_DIR_Y = 0.1;

    
    void update(double& elapsed) {
        point += direction * elapsed;
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
        direction.x = (direction.x > 0 ? -0.5 : 0.5);

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<double> dist(-ABS_MAX_DIR_Y, ABS_MAX_DIR_Y);

        do {
            direction.y = dist(gen);
        } while (std::abs(direction.y) < ABS_MIN_DIR_Y);

        // make base speed
        direction /= direction.calculateMagnitude();
        direction *= ABS_MIN_SPEED;
    }
};