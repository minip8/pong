#pragma once

#include <thread>
#include <atomic>
#include <chrono>

#include "game.h"
class Game;

using namespace std::chrono;


class AI {

private:
    Game& m_Game;
    std::thread m_Thread;
    std::atomic<bool> m_Running;
    std::atomic<Key> m_Move;
    
    int m_TargetPos;
    bool m_TargetCalculated;

private:
    void run();
    int calculateTargetPos();

public:
    AI(Game&);
    ~AI();

    void start();
    void stop();
    bool getRunningStatus() const;
    void updateMove();
    Key getMove() const;

};
