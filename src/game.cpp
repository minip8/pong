#include <ncurses.h>
#include <chrono>
#include <unistd.h>
#include <algorithm>
#include <cmath>
#include <assert.h>
#include <iostream>

#include "game.h"
#include "key.h"
#include "ai.h"
#include "window.h"

static Game* s_Game = nullptr;

Game::Game() :
    m_Ball{},
    m_LeftPaddle{WindowSpecification::HEIGHT / 2 - Paddle::HEIGHT / 2},
    m_RightPaddle{WindowSpecification::HEIGHT / 2 - Paddle::HEIGHT / 2},
    m_PressedKeys{},
    m_LeftScore{0},
    m_RightScore{0},
    m_Running{false}
{
    s_Game = this;
    m_Ball.reset();

    // update controllers
    int numPlayers;
    do {
        std::cout << "Enter 1 for single-player OR 2 for two-player: ";
        std::cin >> numPlayers;
    } while (!(numPlayers == 1 || numPlayers == 2));

    if (numPlayers == 1) setupOnePlayerGame();
    if (numPlayers == 2) setupTwoPlayerGame();
}

Game::~Game() {
    s_Game = nullptr;
}

Game& Game::get() {
    assert(s_Game);
    return *s_Game;
}

void Game::setupOnePlayerGame() {
    m_RightController = std::make_unique<HumanController>(m_rightKeyUp, m_rightKeyDown, m_quitKey);

    static AI ai{get()};
    ai.start();
    m_LeftController = std::make_unique<AIController>(ai);
}

void Game::setupTwoPlayerGame() {
    m_LeftController = std::make_unique<HumanController>(m_leftKeyUp, m_leftKeyDown, m_quitKey);
    m_RightController = std::make_unique<HumanController>(m_rightKeyUp, m_rightKeyDown, m_quitKey);
}

bool Game::processInput() {
    int t = ' ';
    bool input = false;
    while ((t = getch()) != ERR) {
        if (m_LeftController->checkKey(t)) {
            m_LeftController->updateMove(t);
            input = true;
        }
        else if (m_RightController->checkKey(t)) {
            m_RightController->updateMove(t);
            input = true;
        }
    }
    return input;
}

void Game::updateInput() {
    updateInput(m_LeftPaddle, m_LeftController);
    updateInput(m_RightPaddle, m_RightController);
}


// -1 if Left, 0 if NONE, 1 if Right
int Game::checkPaddleCollision() {
    double xn = (m_Ball.point + m_Ball.direction).x;

    if (xn <= 0 && checkPaddleCollision(m_LeftPaddle, m_Ball.point.x))
        return -1;

    if (xn >= WindowSpecification::WIDTH && checkPaddleCollision(m_RightPaddle, WindowSpecification::WIDTH - m_Ball.point.x))
        return 1;

    return 0;
}


bool Game::checkBallVerticalCollision() {
    double yn = (m_Ball.point + m_Ball.direction).y;

    if (yn <= 0) {
        handleBallVerticalCollision(m_Ball.point.y);
        return true;
    }
    
    if (yn >= WindowSpecification::HEIGHT) {
        handleBallVerticalCollision(WindowSpecification::HEIGHT - m_Ball.point.y);
        return true;
    }

    return false;
}

// -1 if Left, 0 if NONE, 1 if Right
int Game::checkLoser() {
    if (0 <= m_Ball.point.x && m_Ball.point.x <= WindowSpecification::WIDTH) return 0;
    return (m_Ball.point.x < 0 ? -1 : 1);
}

// checks ball state
void Game::update(double elapsed) {

    int collision = checkPaddleCollision();
    switch (collision) {
    
    case -1:
    case 1:    
    default:
        break;

    }

    checkBallVerticalCollision();
    
    m_Ball.update(elapsed);

    int loser = checkLoser();
    switch (loser) {
    
    case -1:
        ++m_RightScore;
        m_Ball.reset();
        break;
    
    case 1:
        ++m_LeftScore;
        m_Ball.reset();
        break;

    default:
        break;
    }
}

void Game::render() {
    clear();

    // roof and floor
    for (int j = 0; j < WindowSpecification::WIDTH; ++j) {
        mvaddch(0, j, '-');
        mvaddch(WindowSpecification::HEIGHT, j, '-');
    }

    // paddles
    for (int i = 0; i < Paddle::HEIGHT; ++i) {
        mvaddch(m_LeftPaddle.getPos() + i, 0, '|');
        mvaddch(m_RightPaddle.getPos() + i, WindowSpecification::WIDTH, '|');
    }

    // ball
    mvaddch(
        static_cast<int>(std::round(m_Ball.point.y)),
        static_cast<int>(std::round(m_Ball.point.x)),
        'O'
    );

    // scores
    mvprintw(WindowSpecification::WIDTH >> 1, 0, "Player1: %d | Player2: %d | ` to quit", m_LeftScore, m_RightScore);

    refresh();
}

// thanks chatgpt!
void Game::run() {
    using namespace std::chrono;
    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE); // non-blocking input
    keypad(stdscr, TRUE);

    auto prev = high_resolution_clock::now();

    int framesSinceLastInputUpdate = 0;

    m_Running = true;
    while (isRunning()) {
        auto frameStart = high_resolution_clock::now();
        auto now = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(now - prev).count();
        prev = now;

        // handle input
        bool input = processInput();

        if (input || framesSinceLastInputUpdate > FRAMES_PER_INPUT_UPDATE) {
            updateInput();
            framesSinceLastInputUpdate = 0;
        }

        ++framesSinceLastInputUpdate;

        update(1);

        render();

        // cap FPS
        auto frameEnd = high_resolution_clock::now();
        auto frameTime = duration_cast<microseconds>(frameEnd - frameStart).count();
        long frameDuration = 1000000 / FPS;
        if (frameTime < frameDuration)
            usleep(frameDuration - frameTime);
    }
    endwin();
}

