#pragma once

#include "ball.h"
#include "window.h"

class Paddle {

private:
    int y; // the bottom of the paddle

public:
    void moveUp() {
        if (y > 0)
            --y;
    }

    void moveDown() {
        if (y + HEIGHT < WindowSpecification::HEIGHT)
            ++y;
    }

    int getPos() { return y; }

public:
    static constexpr int HEIGHT = 4;

public:
    Paddle (int initial_y) : y(initial_y) {}
};