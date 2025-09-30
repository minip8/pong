#pragma once
#include "ball.h"
#include "paddle.h"

class Game
{

private:
    Ball m_Ball;
    Paddle m_LeftPaddle;
    Paddle m_RightPaddle;
    int m_LeftScore;
    int m_RightScore;
    bool m_Running;

private:
    bool checkCollision(Paddle& paddle, double x_dist) {
        int paddle_y = paddle.getPos();
        Vec2 pn = m_Ball.point + m_Ball.direction * (x_dist / std::abs(m_Ball.direction.x));
        if (paddle_y <= pn.y && pn.y <= paddle_y + paddle.HEIGHT) {
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

public:
    static constexpr int FPS = 60;
    static constexpr double BALL_UPDATE_INTERVAL = 0.05;

public:
    void processInput(char c);
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