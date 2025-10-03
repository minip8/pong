#pragma once

#include <ncurses.h>
#include <assert.h>
#include <set>

#include "key.h"

class AI;

class Controller {
public:
    virtual ~Controller() = default;

    virtual bool checkKey(int) { return false; }
    virtual void updateMove(int) {}
    virtual Key getMove() = 0;
};

class HumanController : public Controller {
private:
    int m_UpKey;
    int m_DownKey;
    int m_QuitKey;
    Key m_KeyState;

public:
    HumanController(int upKey, int downKey, int quitKey) :
        m_UpKey{upKey},
        m_DownKey{downKey},
        m_QuitKey{quitKey},
        m_KeyState{Key::NONE}
    {}

    bool checkKey(int key) override {
        return key == m_UpKey || key == m_DownKey || key == m_QuitKey;
    }

    void updateMove(int key) override {
        if (key == m_UpKey) m_KeyState = Key::UP;
        else if (key == m_DownKey) m_KeyState = Key::DOWN;
        else if (key == m_QuitKey) m_KeyState = Key::QUIT;
    }
    
    Key getMove() override {
        return m_KeyState;
    }
};

class AIController : public Controller {
private:
    AI& m_AI;

public:
    AIController(AI& ai);

    Key getMove();
};
