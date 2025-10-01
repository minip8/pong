#include <chrono>
#include "ai.h"
#include "game.h"
#include "paddle.h"

AIController::AIController(Game& game) :
    m_Game{game},
    m_Running{false},
    m_Move{AIKey::NONE}
{}


AIController::~AIController() {
    stop();
}

void AIController::start() {
    m_Running = true;
    m_Thread = std::thread(&AIController::run, this);
}

void AIController::stop() {
    m_Running = false;
    if (m_Thread.joinable()) 
        m_Thread.join();
}

AIKey AIController::getMove() const {
    return m_Move.load();
}

void AIController::run() {
    using namespace std::chrono_literals;
    while (m_Running) {
        auto paddlePos = m_Game.getPaddlePos() + Paddle::HEIGHT / 2 + 1;
        auto ballPos = m_Game.getBallPos();
        auto ballDir = m_Game.getBallDir();
        int ballHeight = static_cast<int>(std::round(ballPos.y));

        if (paddlePos < ballHeight)
            m_Move = AIKey::DOWN;
        
        else if (paddlePos > ballHeight)
            m_Move = AIKey::UP;
        
        else
            m_Move = AIKey::NONE;

        std::this_thread::sleep_for(Game::AI_UPDATE_INTERVAL); // ~60fps thinking loop
    }
}
