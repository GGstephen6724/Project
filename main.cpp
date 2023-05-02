/* --------------------------------------------------------
 *  Author: George Stephen
 *  Class: Programming II, COP 3003
 *
 ___       __   _______   ___       ________  ________  _____ ______   _______
|\  \     |\  \|\  ___ \ |\  \     |\   ____\|\   __  \|\   _ \  _   \|\  ___ \
\ \  \    \ \  \ \   __/|\ \  \    \ \  \___|\ \  \|\  \ \  \\\__\ \  \ \   __/|
 \ \  \  __\ \  \ \  \_|/_\ \  \    \ \  \    \ \  \\\  \ \  \\|__| \  \ \  \_|/__
  \ \  \|\__\_\  \ \  \_|\ \ \  \____\ \  \____\ \  \\\  \ \  \    \ \  \ \  \_|\ \
   \ \____________\ \_______\ \_______\ \_______\ \_______\ \__\    \ \__\ \_______\
    \|____________|\|_______|\|_______|\|_______|\|_______|\|__|     \|__|\|_______|
 _________  ________
|\___   ___\\   __  \
\|___ \  \_\ \  \|\  \
     \ \  \ \ \  \\\  \
      \ \  \ \ \  \\\  \
       \ \__\ \ \_______\
        \|__|  \|_______|
 ________  ________  ________   ________
|\   __  \|\   __  \|\   ___  \|\   ____\
\ \  \|\  \ \  \|\  \ \  \\ \  \ \  \___|
 \ \   ____\ \  \\\  \ \  \\ \  \ \  \  ___
  \ \  \___|\ \  \\\  \ \  \\ \  \ \  \|\  \
   \ \__\    \ \_______\ \__\\ \__\ \_______\
    \|__|     \|_______|\|__| \|__|\|_______|

 This project runs a game called Pong. A game using the SFML library that contains
 two paddles and a ball. The objective of the game is to score as many points as
 possible by hitting the ball past your opponents paddle.
 * -------------------------------------------------------- */

#include "pong_defs.h"

// Function declarations (prototypes)
void setup(Ball &ball, Borders &theWalls, MovingBlock &playerPaddle, MovingBlock &AI_paddle);
Direction processInput();
bool update(Direction &input, Ball &ball, float delta, Borders walls, MovingBlock &playerPaddle, bool &started, MovingBlock &AI_paddle); // end update
void render(sf::RenderWindow &window, Ball &ball, float delta, Borders &theWalls, MovingBlock &playerPaddle, MovingBlock &AI_paddle);
bool collisionChecks(bool gameOver, Ball &ball, MovingBlock &playerPaddle, Borders &walls, MovingBlock &AI_paddle);
void moveAIPaddle(Ball &ball, MovingBlock &AI_Paddle);

int main() {
    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hello SFML");
    window.clear(WINDOW_COLOR);
    MovingBlock playerPaddle;
    MovingBlock AI_paddle;
    Ball theBall;
    Borders theWalls;
    setup(theBall, theWalls, playerPaddle, AI_paddle);

    // Timing variables for main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;
    bool started = false;
    bool gameOver = false;
    while (!gameOver)
    {
        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;
        // process events
        sf::Event event;
        while (!gameOver && window.pollEvent(event)) {
            // close requested event: we close the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        }
        // Process user input
        Direction userInput = processInput();
        if (userInput == Exit)
            gameOver = true;
        // Process Updates
        if (delta >= FRAME_RATE) {    // if we have made it at least a full frame time
            update(userInput, theBall, delta, theWalls, playerPaddle, started, AI_paddle);
//            gameOver = update(&userInput, &theBall, theWalls, delta, playerPaddle, started);
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }
        // Render the window
        render(window, theBall, delta, theWalls, playerPaddle, AI_paddle);
    } // end main game loop
    // close the graphics window
    window.close();
    return 0;
}

void setup(Ball &theBall, Borders &theWalls, MovingBlock &playerPaddle, MovingBlock &AI_paddle){

    theWalls.leftWall.left = 0.0;
    theWalls.leftWall.top = 0.0;
    theWalls.leftWall.width = WALL_THICKNESS;
    theWalls.leftWall.height = WINDOW_HEIGHT;
    theWalls.leftWall.rect.setSize(sf::Vector2(theWalls.leftWall.width, theWalls.leftWall.height));
    theWalls.leftWall.rect.setPosition(theWalls.leftWall.left, theWalls.leftWall.top);
    theWalls.leftWall.rect.setFillColor(WALLCOLOR);

    theWalls.rightWall.left = WINDOW_WIDTH - WALL_THICKNESS;
    theWalls.rightWall.top = 0.0;
    theWalls.rightWall.width = WALL_THICKNESS;
    theWalls.rightWall.height = WINDOW_HEIGHT;
    theWalls.rightWall.rect.setSize(sf::Vector2(theWalls.rightWall.width, theWalls.rightWall.height));
    theWalls.rightWall.rect.setPosition(theWalls.rightWall.left, theWalls.rightWall.top);
    theWalls.rightWall.rect.setFillColor(WALLCOLOR);

    theWalls.topWall.left = 0.0;
    theWalls.topWall.top = 0.0;
    theWalls.topWall.width = WINDOW_WIDTH;
    theWalls.topWall.height = WALL_THICKNESS;
    theWalls.topWall.rect.setSize(sf::Vector2(theWalls.topWall.width, theWalls.topWall.height));
    theWalls.topWall.rect.setPosition(theWalls.topWall.left, theWalls.topWall.top);
    theWalls.topWall.rect.setFillColor(WALLCOLOR);

    theWalls.bottomWall.left = 0.0;
    theWalls.bottomWall.top = WINDOW_HEIGHT - WALL_THICKNESS;
    theWalls.bottomWall.width = WINDOW_WIDTH;
    theWalls.bottomWall.height = WALL_THICKNESS;
    theWalls.bottomWall.rect.setSize(sf::Vector2(theWalls.bottomWall.width, theWalls.bottomWall.height));
    theWalls.bottomWall.rect.setPosition(theWalls.bottomWall.left, theWalls.bottomWall.top);
    theWalls.bottomWall.rect.setFillColor(WALLCOLOR);

    playerPaddle.block.left = PADDLE_THICKNESS;
    playerPaddle.block.top = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0;
    playerPaddle.block.width = PADDLE_THICKNESS;
    playerPaddle.block.height = PADDLE_HEIGHT;
    playerPaddle.block.wallColor = PADDLE_COLOR;
    playerPaddle.block.rect.setSize(sf::Vector2f(playerPaddle.block.width, playerPaddle.block.height));
    playerPaddle.block.rect.setFillColor(PADDLE_COLOR);
    playerPaddle.velocityX = 0.0;
    playerPaddle.velocityY = 0.0;

    AI_paddle.block.left = WINDOW_WIDTH - 2 * PADDLE_THICKNESS - WALL_THICKNESS;
    AI_paddle.block.top = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0;
    AI_paddle.block.width = PADDLE_THICKNESS;
    AI_paddle.block.height = PADDLE_HEIGHT;
    AI_paddle.block.wallColor = sf::Color::Green;
    AI_paddle.block.rect.setSize(sf::Vector2f(AI_paddle.block.width, AI_paddle.block.height));
    AI_paddle.block.rect.setFillColor(sf::Color::Green);
    AI_paddle.velocityX = 0.0;
    AI_paddle.velocityY = 0.0;

    theBall.radius = BALL_RADIUS;
    theBall.coordinateX = playerPaddle.block.left + playerPaddle.block.width + theBall.radius + 1;
    theBall.coordinateY = playerPaddle.block.top + (playerPaddle.block.height / 2.0);
    theBall.velocityX = 0.0;
    theBall.velocityY = 0.0;
    theBall.color = BALL_COLOR;

}

bool checkCollision(Ball &theBall, Block &theWalls, MovingBlock &playerPaddle, MovingBlock &AI_Paddle){
    int collision = 0;
    float ballLeft = theBall.coordinateX - theBall.radius;
    float ballTop = theBall.coordinateY - theBall.radius;
    float ballRight = theBall.coordinateX + theBall.radius;
    float ballBottom = theBall.coordinateY + theBall.radius;

    float boxLeft = theWalls.left;
    float boxTop = theWalls.top;
    float boxRight = theWalls.left + theWalls.width;
    float boxBottom = theWalls.top + theWalls.height;

    float paddleLeft = playerPaddle.block.left;
    float paddleTop = playerPaddle.block.top;
    float paddleRight = playerPaddle.block.left + playerPaddle.block.width;
    float paddleBottom = playerPaddle.block.top + playerPaddle.block.height;

    float aiPaddleLeft = AI_Paddle.block.left;
    float aiPaddleTop = AI_Paddle.block.top;
    float aiPaddleRight = AI_Paddle.block.left + AI_Paddle.block.width;
    float aiPaddleBottom = AI_Paddle.block.top + AI_Paddle.block.height;

    if(ballLeft < aiPaddleLeft &&
       ballRight > aiPaddleRight &&
       ballTop < aiPaddleBottom &&
       ballBottom > aiPaddleTop ){
        return true;
    }

    if (paddleTop > boxBottom &&
        paddleBottom < boxTop){
        collision = 2;
        return true;
    }

    if(ballLeft < boxRight &&
       ballRight > boxLeft &&
       ballTop < boxBottom &&
       ballBottom > boxTop ){
        collision = 1;
    }
    return collision;
}

Direction processInput() {
    Direction input = None;  // no input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        input = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        input = Exit;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        input = Start;
    }
    return input;
} // end getUserInput

void render(sf::RenderWindow &window, Ball &ball, float delta, Borders &theWalls, MovingBlock &playerPaddle, MovingBlock &AI_paddle) {
    // clear the window with the background color
    window.clear(WINDOW_COLOR);

    // draw the ball
    sf::CircleShape circle;
    circle.setFillColor(BALL_COLOR);
    circle.setRadius(ball.radius);
    // set screen coordinates relative to the center of the circle
    circle.setOrigin(ball.radius, ball.radius);

    // Ball Position
    float xPosition;
    float yPosition;
    xPosition = ball.coordinateX + ball.velocityX * delta;
    yPosition = ball.coordinateY + ball.velocityY * delta;
    circle.setPosition(xPosition, yPosition);

    // Paddle Position
    float X;
    float Y;
    X = playerPaddle.block.left + playerPaddle.velocityX * delta;
    Y = playerPaddle.block.top + playerPaddle.velocityY * delta;
    playerPaddle.block.rect.setPosition(X, Y);


    // AI Paddle Position
    float AI_X;
    float AI_Y;
    AI_X = AI_paddle.block.left + AI_paddle.velocityX * delta;
    AI_Y = AI_paddle.block.top + AI_paddle.velocityY * delta;
    AI_paddle.block.rect.setPosition(AI_X, AI_Y);
    window.draw(AI_paddle.block.rect);
    AI_paddle.block.rect.setFillColor(PADDLE_COLOR);

    // Window Drawing
    window.draw(theWalls.leftWall.rect);
    window.draw(theWalls.rightWall.rect);
    window.draw(theWalls.topWall.rect);
    window.draw(theWalls.bottomWall.rect);

    window.draw(AI_paddle.block.rect);
    window.draw(playerPaddle.block.rect);
    window.draw(circle);
    // show the new window
    window.display();
}

bool collisionChecks(bool gameOver, Ball &ball, MovingBlock &playerPaddle, Borders &walls, MovingBlock &AI_paddle) {
    gameOver = false;

    if(checkCollision(ball, playerPaddle.block, playerPaddle, AI_paddle)){
        // If hit
        ball.velocityX *= - 1;
        ball.coordinateX = playerPaddle.block.left + playerPaddle.block.width + ball.radius + 1;
    }

    if(checkCollision(ball, playerPaddle.block, playerPaddle, AI_paddle)){
        // If hit
        ball.velocityX *= - 1;
        ball.coordinateX = AI_paddle.block.left + AI_paddle.block.width + ball.radius + 1;
    }

    else if(checkCollision(ball, walls.leftWall, playerPaddle, AI_paddle)) {
        ball.velocityX *= -1;
        ball.coordinateX = walls.leftWall.left + walls.leftWall.width + ball.radius;
        gameOver = true;
    }

    else if(checkCollision(ball, walls.rightWall, playerPaddle, AI_paddle)){
        ball.velocityX *= -1;
        ball.coordinateX = walls.rightWall.left - ball.radius -1;
        gameOver = true;
    };

    if(checkCollision(ball, walls.topWall, playerPaddle, AI_paddle)){
        ball.velocityY *= -1;
    }

    else if(checkCollision(ball, walls.bottomWall, playerPaddle, AI_paddle)){
        ball.velocityY *= -1;
        ball.coordinateY = walls.bottomWall.top - ball.radius -1;
    }

    float paddleCenter = AI_paddle.block.top + AI_paddle.block.height / 2.0;
    float ballCenter = ball.coordinateX + ball.coordinateY;

    if (ballCenter < paddleCenter) {
        AI_paddle.velocityY = -PADDLE_SPEED;
    }

    else if (ballCenter > paddleCenter) {
        AI_paddle.velocityY = +PADDLE_SPEED;
    }

    else {
        AI_paddle.velocityY = 0.0;
    }
    return gameOver;
}

bool update(Direction &input, Ball &ball, float delta, Borders walls, MovingBlock &playerPaddle,
       bool &started, MovingBlock &AI_paddle) {
    // adjust velocity directions for user input
    bool gameOver = false;
    if (input) {
        switch (input) {
            case 1: // Up
                playerPaddle.velocityY -= PADDLE_SPEED;
                break;
            case 2: // Down
                playerPaddle.velocityY += PADDLE_SPEED;
                break;
            case 3:
                if(!started){
                    ball.velocityY = BALL_SPEED_Y;
                    ball.velocityX = BALL_SPEED_X;
                    if ((int(delta * 10) & 1) % 2) {
                        ball.velocityY *= -1;
                    }
                    started = true;
                }
                break;

        }
        // clear input
        input = None;
    }

    moveAIPaddle(ball, AI_paddle);

    // Checks the "started" flag variable
    if (started == 1) {
        ball.velocityX * delta + ball.coordinateX;
        ball.velocityY * delta + ball.coordinateY;
        if(ball.coordinateY < AI_paddle.block.top + AI_paddle.block.height){
            AI_paddle.velocityY - PADDLE_SPEED;
            playerPaddle.block.top + playerPaddle.block.height / 2.0;
        }
        else if(ball.coordinateY > playerPaddle.block.top + playerPaddle.block.height / 2.0){
            AI_paddle.velocityY + PADDLE_SPEED;
        }
    }
    else{
        ball.coordinateX = playerPaddle.block.left + playerPaddle.block.width + ball.radius + 1;
        ball.coordinateY = playerPaddle.block.top + (playerPaddle.block.height / 2.0);
    }

    AI_paddle.block.top += AI_paddle.velocityY * delta;
    playerPaddle.block.top += playerPaddle.velocityY * delta;

    // location of the ball for speed * time
    ball.coordinateX += ball.velocityX * delta;
    ball.coordinateY += ball.velocityY * delta;

    collisionChecks(gameOver, ball, playerPaddle, walls, AI_paddle);
    return gameOver;
}

void moveAIPaddle(Ball &ball, MovingBlock &AI_paddle) {
    float paddleCenter = AI_paddle.block.top + AI_paddle.block.height / 2.0;
    float ballCenter = ball.coordinateX + ball.coordinateY;

    if (ballCenter < paddleCenter) {
        AI_paddle.velocityY = -PADDLE_SPEED;
    }

    else if (ballCenter > paddleCenter) {
        AI_paddle.velocityY = +PADDLE_SPEED;
    }

    else {
        AI_paddle.velocityY = 0.0;
    }
}



