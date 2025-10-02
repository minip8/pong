#include <chrono>
#include <assert.h>
#include "ai.h"
#include "game.h"
#include "paddle.h"
#include "window.h"

AIController::AIController(Game& game) :
    m_Game{game},
    m_Running{false},
    m_Move{AIKey::NONE},
    m_TargetPos{},
    m_TargetCalculated{false}
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

// assumes ball is travelling towards AI paddle (left)
int AIController::calculateTargetPos() {
    auto ballPos{m_Game.getBallPos()};
    auto ballDir{m_Game.getBallDir()};
    double ballHeight{ballPos.y};

    assert(ballDir.x < 0);

    auto dx{ballPos.x};
    auto totalChange{dx / std::abs(ballDir.x) * ballDir};
    double dy{totalChange.y};
    double finalPos{std::abs(dy + ballHeight)};

    int verticalGap{WindowSpecification::HEIGHT};
    
    int bounces{static_cast<int>(finalPos) / verticalGap};
    finalPos -= bounces * verticalGap;

    if (bounces & 1)
        finalPos = verticalGap - finalPos;
    
    return finalPos;
}

void AIController::updateMove() {
    assert(m_TargetCalculated);

    auto paddlePos = m_Game.getPaddlePos() + Paddle::HEIGHT / 2;

    if (paddlePos < m_TargetPos)
        m_Move = AIKey::DOWN;
    
    else if (paddlePos > m_TargetPos)
        m_Move = AIKey::UP;
    
    else
        m_Move = AIKey::NONE;
}

void AIController::run() {
    using namespace std::chrono_literals;
    while (m_Running) {
        if (!m_TargetCalculated && m_Game.getBallDir().x < 0) {
            m_TargetPos = calculateTargetPos();
            m_TargetCalculated = true;
        }
        if (m_TargetCalculated && m_Game.getBallDir().x > 0) {
            m_TargetCalculated = false;
        }

        if (m_TargetCalculated)
            updateMove();

        std::this_thread::sleep_for(Game::AI_UPDATE_INTERVAL);
    }
}
