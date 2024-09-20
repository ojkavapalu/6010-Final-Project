//
//  Game.cpp
//  Pong
//
//  Created by Jose Bonilla on 9/20/24.
//

#include "Game.hpp"

// Constructor: Initializes the paddle's position, size, and texture
Paddle::Paddle(float x, float y, const std::string& textureFile) {
    originalHeight = 100.f;  // Set the original height of the paddle
    rec.setSize({25.f, originalHeight});  // Set the size of the paddle (width = 25, height = originalHeight)
    rec.setPosition(x, y);  // Set the initial position of the paddle
    speed = 1400.f;  // Set the paddle's movement speed

    // Load the texture from a file and apply it to the paddle's shape
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error loading paddle texture" << std::endl;  // Display an error if the texture fails to load
    }
    rec.setTexture(&texture);  // Apply the texture to the paddle
}

// Move the paddle up
void Paddle::moveUp(float deltaTime) {
    if (rec.getPosition().y > 0)  // Ensure the paddle doesn't move above the top of the window
        rec.move(0, -speed * deltaTime);  // Move the paddle upward
}

// Move the paddle down
void Paddle::moveDown(float deltaTime, float windowHeight) {
    if (rec.getPosition().y + rec.getSize().y < windowHeight)  // Ensure the paddle doesn't move below the bottom of the window
        rec.move(0, speed * deltaTime);  // Move the paddle downward
}

// Shrink the paddle by reducing its height by 1/5 of its original size
void Paddle::shrink() {
    float newHeight = rec.getSize().y - (originalHeight / 5);  // Calculate the new height after shrinking
    if (newHeight > 0) {
        rec.setSize({25.f, newHeight});  // Update the paddle size with the new height
    }
}

// Ball class implementation

// Constructor: Initializes the ball's position, size, speed, and texture
Ball::Ball(float x, float y, const std::string& textureFile) {
    circ.setRadius(10.f);  // Set the radius of the ball
    circ.setPosition(x, y);  // Set the initial position of the ball
    speed = 250.f;  // Set the speed of the ball
    velocity = { -speed, -speed };  // Initialize the ball's movement direction

    // Load the texture from a file and apply it to the ball's shape
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error loading ball texture" << std::endl;  // Display an error if the texture fails to load
    }
    circ.setTexture(&texture);  // Apply the texture to the ball
}

// Move the ball based on its velocity and deltaTime
void Ball::move(float deltaTime) {
    circ.move(velocity * deltaTime);  // Update the ball's position based on its velocity
}

// Reset the ball's position and velocity after a point is scored
void Ball::reset(float x, float y) {
    circ.setPosition(x, y);  // Reset the ball's position
    velocity = { -speed, -speed };  // Reset the ball's velocity
}

// Check if the ball hits the top or bottom wall and reverse its direction
void Ball::checkCollisionWithWalls(float windowHeight) {
    if (circ.getPosition().y <= 0 || circ.getPosition().y >= windowHeight - circ.getRadius() * 2) { //
        velocity.y = -velocity.y;  // Reverse the vertical velocity
    }
}

// Check if the ball collides with a paddle and reverse its direction
void Ball::checkCollisionWithPaddle(const sf::RectangleShape& paddle) {
    if (circ.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
        velocity.x = -velocity.x;  // Reverse the horizontal velocity
    }
}

// Check if the ball is out of bounds on the x-axis
bool Ball::isOutOfBounds(float windowWidth) {
    return circ.getPosition().x <= 0 || circ.getPosition().x >= windowWidth;
}

// Game class implementation

// Constructor: Initializes the game
Game::Game()
    : window(sf::VideoMode(800, 600), "Pong"),
      leftPaddle(50.f, 300.f - 50.f, "/Users/jbon/FinalProject/6010-Final-Project/paddle_texture.jpeg"),
      rightPaddle(750.f - 25.f, 300.f - 50.f, "/Users/jbon/FinalProject/6010-Final-Project/paddle_texture.jpeg"),
      ball(400.f, 300.f, "/Users/jbon/FinalProject/6010-Final-Project/fireball_texture.jpeg"),
      leftScore(0), rightScore(0),
      leftPaddleMovingUp(true),
      gameOver(false), scoreLimit(5) {

    window.setFramerateLimit(60);  // Set the frame rate limit

    // Load font
    if (!font.loadFromFile("/Users/jbon/FinalProject/6010-Final-Project/BUBBLEGUMS.TTF")) {
        std::cerr << "Error loading font" << std::endl;
    }

    // Set up the left player's score text
    leftScoreText.setFont(font);
    leftScoreText.setCharacterSize(30);
    leftScoreText.setFillColor(sf::Color::White);
    leftScoreText.setPosition(200.f, 20.f);

    // Set up the right player's score text
    rightScoreText.setFont(font);
    rightScoreText.setCharacterSize(30);
    rightScoreText.setFillColor(sf::Color::White);
    rightScoreText.setPosition(600.f, 20.f);

    // Set up the winner message text
    winnerText.setFont(font);
    winnerText.setCharacterSize(30);
    winnerText.setFillColor(sf::Color::Yellow);
    winnerText.setPosition(200.f, 300.f);

    updateScoreText();  // Initialize the score text

    // Create the dashed line in the middle of the screen
    float dashHeight = 10.f;
    float dashWidth = 5.f;
    float gap = 10.f;
    float xPosition = (window.getSize().x / 2) - (dashWidth / 2);  // Calculate the X position to center the dashes
    for (float yPosition = 0; yPosition < window.getSize().y; yPosition += dashHeight + gap) {
        
          sf::RectangleShape dash(sf::Vector2f(dashWidth, dashHeight));  // Create a rectangle to represent a dash
          dash.setPosition(xPosition, yPosition);  // Set the position of the dash in the middle of the screen
          dash.setFillColor(sf::Color::White);  // Set the dash color to white
          dashedLine.push_back(dash);  // Add the dash to the dashedLine vector
        
      }
    }
void Game::run() {
    sf::Clock clock;  // Clock to measure deltaTime

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();  // Measure time passed since the last frame

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();  // Close the window and stop the game
            }

            if (!gameOver) {
                
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
                    rightPaddle.moveUp(deltaTime);  // Move the right paddle up when the up arrow is pressed
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                    rightPaddle.moveDown(deltaTime, window.getSize().y);  // Move the right paddle down
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W) {
                    leftPaddle.moveUp(deltaTime);  // Move the left paddle up when 'W' is pressed
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                    leftPaddle.moveDown(deltaTime, window.getSize().y);  // Move the left paddle down when 'S' is pressed
                }
            }
        }

        if (!gameOver) {  // Only update the game if it's not over
            update(deltaTime);  // Update the game state
        } else {
            // If the game is over, display the winner message for 3 seconds
            if (gameOverClock.getElapsedTime().asSeconds() > 3.f) {
                window.close();  // Close the window after 3 seconds
            }
        }

        render();  // Render all the objects on the window
    }
}

// Update game state based on elapsed time
void Game::update(float deltaTime) {
    ball.move(deltaTime);  // Move the ball

    ball.checkCollisionWithWalls(window.getSize().y);  // Check for collisions between the ball and the top/bottom walls
    ball.checkCollisionWithPaddle(leftPaddle.rec);   // Check for collisions between the ball and the left paddle
    ball.checkCollisionWithPaddle(rightPaddle.rec);  // Check for collisions between the ball and the right paddle

    // Check if the ball is out of bounds and update the score
    if (ball.isOutOfBounds(window.getSize().x)) {
        if (ball.circ.getPosition().x <= 0) {  // If the ball goes out on the left (right player scores)
            rightScore++;  // Increase right player's score
            leftPaddle.shrink();  // Shrink the left paddle
            updateScoreText();  // Update the score display
            checkWin();  // Check if someone has won the game
            
        } else if (ball.circ.getPosition().x >= window.getSize().x - ball.circ.getRadius() * 2) {  // If the ball goes out on the right (left player scores)
            leftScore++;  // Increase left player's score
            rightPaddle.shrink();  // Shrink the right paddle
            updateScoreText();  // Update the score display
            checkWin();  // Check if someone has won the game
        }

        ball.reset(400.f, 300.f);  // Reset the ball to the center of the screen
    }
}

// Update the score display
void Game::updateScoreText() {
    leftScoreText.setString(std::to_string(leftScore));  // Convert leftScore to string and set it in leftScoreText
    rightScoreText.setString(std::to_string(rightScore));  // Convert rightScore to string and set it in rightScoreText
}

// Check if a player has won the game
void Game::checkWin() {
    if (leftScore >= scoreLimit) {  // If the left player reaches the score limit
        winnerText.setString("Left Player Wins!");  // Set the winner text
        gameOver = true;  // Set the game over flag
        gameOverClock.restart();  // Start the game over clock
    } else if (rightScore >= scoreLimit) {  // If the right player reaches the score limit
        winnerText.setString("Right Player Wins!");  // Set the winner text
        gameOver = true;  // Set the game over flag
        gameOverClock.restart();  // Start the game over clock
    }
}

// Render all game objects on the screen
void Game::render() {
    window.clear(sf::Color::Black);  // Clear the window with a black background

    // Draw the dashed line in the middle of the screen
    for (const auto& dash : dashedLine) {
        window.draw(dash);  // Draw each dash (rectangle) in the vector
    }

    window.draw(leftPaddle.rec);   // Draw the left paddle
    window.draw(rightPaddle.rec);  // Draw the right paddle
    window.draw(ball.circ);        // Draw the ball
    window.draw(leftScoreText);    // Draw the left player's score
    window.draw(rightScoreText);   // Draw the right player's score

    if (gameOver) {
        window.draw(winnerText);  // Draw the winner message when the game is over
    }

    window.display();  // Display everything rendered in the window
}
