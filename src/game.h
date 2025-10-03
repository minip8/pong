#pragma once

#include <chrono>
#include <set>

#include "ball.h"
#include "paddle.h"
#include "key.h"
#include "ai.h"
#include "controller.h"

enum class Key;
using namespace std::chrono;

class Game
{

private:
    Ball m_Ball;
    Paddle m_LeftPaddle;
    Paddle m_RightPaddle;
    std::unique_ptr<Controller> m_LeftController;
    std::unique_ptr<Controller> m_RightController;
    static constexpr int m_leftKeyUp = 'q';
    static constexpr int m_leftKeyDown = 'a';
    static constexpr int m_rightKeyUp = 'p';
    static constexpr int m_rightKeyDown = 'l';
    static constexpr int m_quitKey = '`';
    std::set<int> m_PressedKeys;
    int m_LeftScore;
    int m_RightScore;
    bool m_Running;

private:
    bool checkPaddleCollision(Paddle& paddle, double x_dist) {
        int paddle_y = paddle.getPos();
        Vec2 pn = m_Ball.point + m_Ball.direction * (x_dist / std::abs(m_Ball.direction.x));
        if (paddle_y <= pn.y && pn.y < paddle_y + paddle.HEIGHT) {
            handlePaddleCollision(pn);
            return true;
        }
        return false;
    }

    void handlePaddleCollision(Vec2<double>& collisionPoint) {
        m_Ball.direction.reflect_y();
        m_Ball.point = collisionPoint + m_Ball.direction;
        m_Ball.direction *= -1;
        m_Ball.increase_speed();
    }

    void handleBallVerticalCollision(double y_dist) {
        Vec2 collisionPoint = m_Ball.point + m_Ball.direction * (y_dist / std::abs(m_Ball.direction.y));
        m_Ball.direction.reflect_x();
        m_Ball.point = collisionPoint + m_Ball.direction;
        m_Ball.direction *= -1;
    }

    void setupOnePlayerGame();

    void setupTwoPlayerGame();

protected:
    void updateInput(Paddle& paddle, std::unique_ptr<Controller>& controller) {
        Key cur_state = controller->getMove();
        
        switch (cur_state) {
        
        case Key::UP: paddle.moveUp(); break;

        case Key::DOWN: paddle.moveDown(); break;

        case Key::QUIT: m_Running = false; break;

        default: break;

        }
    }

public:
    static constexpr int FPS = 60;
    static constexpr int FRAMES_PER_INPUT_UPDATE = 2;
    static constexpr auto BALL_UPDATE_INTERVAL = duration<double>(2.0 / FPS); // this is not used
    static constexpr auto AI_UPDATE_INTERVAL = duration<double>(1.0 / FPS);

public:
    auto getPaddlePos() { return m_LeftPaddle.getPos(); }
    auto getBallPos() { return m_Ball.point; }
    auto getBallDir() { return m_Ball.direction; }

public:
    Game& get();
    bool processInput();
    void updateInput();
    void update(double elapsed);
    void render();
    int checkPaddleCollision();
    bool checkBallVerticalCollision();
    int checkLoser();
    bool isRunning() { return m_Running; }
    void run();

public:
    Game();
    ~Game();

};