#include <ncurses.h>
#include <chrono>
#include <unistd.h>
#include <algorithm>
#include <cmath>
#include <assert.h>
#include "game.h"
#include "key.h"
#include "ai.h"

static Game* s_Game = nullptr;

Game::Game() :
    m_Ball{},
    m_LeftPaddle{WindowSpecification::HEIGHT / 2 - Paddle::HEIGHT / 2},
    m_RightPaddle{WindowSpecification::HEIGHT / 2 - Paddle::HEIGHT / 2},
    m_LeftKeyState{Key::NONE},
    m_RightKeyState{Key::NONE},
    m_LeftScore{0},
    m_RightScore{0},
    m_Running{false}
{
    s_Game = this;
    m_Ball.reset();
}

Game::~Game() {
    s_Game = nullptr;
}

void Game::processInput(char c) {
    switch (c) {

    case '`':
        m_Running = false;
        break;
    
    case 'q':
        m_LeftKeyState = Key::UP;
        break;
        
    case 'a':
        m_LeftKeyState = Key::DOWN;
        break;
        
    case 'p':
        m_RightKeyState = Key::UP;
        break;
        
    case 'l':
        m_RightKeyState = Key::DOWN;
        break;

    default:
        break;
    }
}

void Game::updateInput(AIKey&& key) {
    switch (key) {

    case AIKey::UP:
        m_LeftPaddle.moveUp();
        break;
    
    case AIKey::DOWN:
        m_LeftPaddle.moveDown();
        break;
    
    default:
        break;
    }
}

void Game::updateInput() {
    updateInput(m_LeftPaddle, m_LeftKeyState);
    updateInput(m_RightPaddle, m_RightKeyState);
}


// -1 if Left, 0 if NONE, 1 if Right
int Game::checkCollision() {
    double xn = (m_Ball.point + m_Ball.direction).x;
    if (xn <= 0 && checkCollision(m_LeftPaddle, m_Ball.point.x)) return -1;
    if (xn >= WindowSpecification::WIDTH && checkCollision(m_RightPaddle, WindowSpecification::WIDTH - m_Ball.point.x)) return 1;
    return 0;
}

// -1 if Left, 0 if NONE, 1 if Right
int Game::checkLoser() {
    if (0 <= m_Ball.point.x && m_Ball.point.x <= WindowSpecification::WIDTH) return 0;
    return (m_Ball.point.x < 0 ? -1 : 1);
}

// checks ball state
void Game::update(double elapsed) {

    int collision = checkCollision();
    switch (collision) {
    
    case -1:
    case 1:    
    default:
        break;

    }
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

    AIController ai{*s_Game};
    ai.start();

    m_Running = true;
    while (isRunning()) {
        auto frameStart = high_resolution_clock::now();
        auto now = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(now - prev).count();
        prev = now;

        // handle input
        char ch;
        bool input = false;
        while ((ch = static_cast<char>(getch())) != ERR) {
            processInput(ch);
            input = true;
        }

        if (input || framesSinceLastInputUpdate > FRAMES_PER_INPUT_UPDATE) {
            updateInput();
            framesSinceLastInputUpdate = 0;
        }

        ++framesSinceLastInputUpdate;
    
        // ai
        updateInput(ai.getMove());

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

Game Game::get() {
    assert(s_Game);
    return *s_Game;
}