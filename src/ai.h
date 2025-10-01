#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
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

private:
    void run();

public:
    AIController(Game&);
    ~AIController();

    void start();
    void stop();
    void update();
    AIKey getMove() const;

};
