#pragma once

#include <thread>
#include <atomic>
#include <chrono>

#include "game.h"
class Game;

using namespace std::chrono;


enum class AIKey {
    NONE,
    UP,
    DOWN
};

class AIController {

private:
    Game& m_Game;
    std::thread m_Thread;
    std::atomic<bool> m_Running;
    std::atomic<AIKey> m_Move;
    
    int m_TargetPos;
    bool m_TargetCalculated;

private:
    void run();
    int calculateTargetPos();

public:
    AIController(Game&);
    ~AIController();

    void start();
    void stop();
    void updateMove();
    AIKey getMove() const;

};
