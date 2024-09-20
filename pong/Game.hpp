//
//  Game.hpp
//  Pong
//
//  Created by Jose Bonilla on 9/20/24.
//

#ifndef Game_hpp
#define Game_hpp
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>


class Paddle {
public:
    sf::RectangleShape rec;  // Rectangle shape for the paddle
    sf::Texture texture;     // Texture to apply to the paddles shape
    float speed;
    float originalHeight;    // Original height of the paddle, we use for shrinking height

    // Constructor: Initializes the paddle's position, size, and texture
    Paddle(float x, float y, const std::string& textureFile);

    // Move the paddle up
    void moveUp(float deltaTime);

    // Move the paddle down
    void moveDown(float deltaTime, float windowHeight);

    // Shrink the paddle by reducing its height
    void shrink();
};


class Ball {
public:
    sf::CircleShape circ;     // Circle shape for the ball
    sf::Texture texture;      // Texture of ball
    sf::Vector2f velocity;
    float speed;

    // Constructor: Initializes the ball's position, size, speed, and texture
    Ball(float x, float y, const std::string& textureFile);

    // Move the ball based on its velocity and deltaTime
    void move(float deltaTime);

    // Reset the ball's position and velocity after a point is scored
    void reset(float x, float y);

    // Check if the ball hits the top or bottom wall and reverse its direction
    void checkCollisionWithWalls(float windowHeight);

    // Check if the ball collides with a paddle
    void checkCollisionWithPaddle(const sf::RectangleShape& paddle);

    // Check if the ball is out of bounds on the x-axis
    bool isOutOfBounds(float windowWidth);
};


class Game {
public:
    Game(); // Constructor that initializes the game
    void run(); // Run the game loop

private:
    sf::RenderWindow window;
    Paddle leftPaddle;
    Paddle rightPaddle;
    Ball ball;
    int leftScore;
    int rightScore;
    const int scoreLimit;
    bool leftPaddleMovingUp;
    
    sf::Font font;
    sf::Text leftScoreText;
    sf::Text rightScoreText;
    sf::Text winnerText;     
    
    bool gameOver;
    
    sf::Clock gameOverClock;
    
    std::vector<sf::RectangleShape> dashedLine;

    // Update the game state
    void update(float deltaTime);

    // Update the score text
    void updateScoreText();

    // Check if a player has won
    void checkWin();

    // Render all objects on the window
    void render();
};
#endif /* Game_hpp */
