#pragma once
#include "ball.h"
#include "paddle.h"
#include "keystate.h"

class Game
{

private:
    Ball m_Ball;
    Paddle m_LeftPaddle;
    Paddle m_RightPaddle;
    KeyState m_LeftKeyStatePrev;
    KeyState m_LeftKeyStateCur;
    KeyState m_RightKeyStatePrev;
    KeyState m_RightKeyStateCur;
    int m_LeftScore;
    int m_RightScore;
    bool m_Running;

private:
    bool checkCollision(Paddle& paddle, double x_dist) {
        int paddle_y = paddle.getPos();
        Vec2 pn = m_Ball.point + m_Ball.direction * (x_dist / std::abs(m_Ball.direction.x));
        if (paddle_y <= pn.y && pn.y < paddle_y + paddle.HEIGHT) {
            handleCollision(pn);
            return true;
        }
        return false;
    }

    void handleCollision(Vec2<double>& collision_point) {
        m_Ball.direction.reflect_y();
        m_Ball.point = collision_point + m_Ball.direction;
        m_Ball.direction *= -1;
        m_Ball.increase_speed();
    }

protected:
    void updateInput(Paddle& paddle, KeyState& prev_state, KeyState& cur_state) {
        if (cur_state.up) paddle.moveUp();
        if (cur_state.down) paddle.moveDown();
    }

public:
    static constexpr int FPS = 60;
    static constexpr int FRAMES_PER_INPUT_UPDATE = 2;
    static constexpr double BALL_UPDATE_INTERVAL = 0.05;

public:
    Game get();
    void processInput(char);
    void updateInput();
    void update(double elapsed);
    void render();
    int checkCollision();
    int checkLoser();
    bool isRunning() { return m_Running; }
    void run();

public:
    Game();
    ~Game();

};