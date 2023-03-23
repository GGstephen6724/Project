/* --------------------------------------------------------
 *  Author: George Stephen
 *  Class: Programming II, COP 3003
 * -------------------------------------------------------- */
#include <SFML/Graphics.hpp>

#ifndef UNTITLED_PONG_DEFS_H
#define UNTITLED_PONG_DEFS_H

// Global Constants
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const sf::Color WINDOW_COLOR = sf::Color::Black;
const float FRAME_RATE = (1.0/30.0) * 1000.0;       // FPS in ms
const sf::Color BALL_COLOR = sf::Color::White;
// speed that the can accelerate at to span window in
// n-seconds (in ms) broken up for each frame
const float BALL_RADIUS = 10.0;
const float SPEED_TIME = (3.0 * 1000.0) * 30.0;     //
const float BALL_SPEED_X = BALL_RADIUS * 25.0 / 1000.0;    // speed horizontally
const float BALL_SPEED_Y = BALL_RADIUS * 23.0 / 1000.0;   // span  vertically
const float WALL_THICKNESS = 2.0;
const sf::Color WALLCOLOR = sf::Color::Magenta;
float const PADDLE_HEIGHT = 80.0;
float const PADDLE_THICKNESS = 10.0;
const sf::Color PADDLE_COLOR = sf::Color:: White;
float const PADDLE_SPEED = PADDLE_HEIGHT / 10.0 / 1000.0;

// Type definitions

enum Direction{
    Exit = -1,
    None = 0,
    Left = 4,
    Up = 1,
    Right = 5,
    Down = 2,
    Start = 3
};

struct Ball {
    float radius;
    float coordinateX;
    float coordinateY;
    float velocityX;
    float velocityY;
    sf::Color color;
};

struct Block{
    float left;
    float top;
    float width;
    float height;
    sf::Color wallColor;
    sf::RectangleShape rect;
};

struct MovingBlock{
    Block block;
    float velocityX;
    float velocityY;
};

struct Borders{
    Block leftWall;
    Block topWall;
    Block rightWall;
    Block bottomWall;
};

#endif //UNTITLED_PONG_DEFS_H



