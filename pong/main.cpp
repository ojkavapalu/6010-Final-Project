//
//  main.cpp
//  pong
//
//  Created by Jose Bonilla on 9/17/24.
//

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <memory>


class Paddle {
    
public:
    sf::RectangleShape rec;  // Rectangle shape for the paddle
    sf::Texture texture;       // Texture to apply to the paddles shape
    float speed;               // Speed of the paddle movement
    float originalHeight;      // Original height of the paddle, using for shrinking paddle

    // Constructor: Initializes the paddles position, size, and texture
    Paddle(float x, float y, const std::string& textureFile) {
        originalHeight = 100.f;  // Set the original height of the paddle
        rec.setSize({25.f, originalHeight});  // Set the size of the paddle (width = 25, height = originalHeight)
        rec.setPosition(x, y);  // Set the initial position of the paddle
        speed = 900.f;  // Set the paddle's movement speed.

        // Load the texture from a file and apply it to the paddle's shape.
        if (!texture.loadFromFile(textureFile)) {
            std::cerr << "Error loading paddle texture" << std::endl;  // Display an error if the texture fails to load
        }
        rec.setTexture(&texture);  // Apply the texture to the paddle.
    }

    // Move the paddle up by adjusting its position.
    void moveUp(float deltaTime) {
        if (rec.getPosition().y > 0)  // Ensure the paddle doesn't move above the top of the window.
            rec.move(0, -speed * deltaTime);  // Move the paddle upward (speed * deltaTime controls the smooth movement).
    }

    // Move the paddle down by adjusting its position.
    void moveDown(float deltaTime, float windowHeight) {
        if (rec.getPosition().y + rec.getSize().y < windowHeight)  // Ensure the paddle doesn't move below the bottom of the window.
            rec.move(0, speed * deltaTime);  // Move the paddle downward (speed * deltaTime controls the smooth movement).
    }

    // Shrink the paddle by reducing its height by 1/4 of its original size.
    void shrink() {
        float newHeight = rec.getSize().y - (originalHeight / 5);  // Calculate the new height after shrinking.
        if (newHeight > 0) {
            rec.setSize({25.f, newHeight});  // Update the paddle size with the new height.
        }
    }
};


class Ball {
public:
    sf::CircleShape circ;
    sf::Texture texture;     // Texture to apply to the ball.
    sf::Vector2f velocity;   // Velocity vector for the ball, determines movement speed and direction
    float speed; //speed of ball

    // Constructor: Initializes the ball's position, size, speed, and texture
    Ball(float x, float y, const std::string& textureFile) {
        circ.setRadius(10.f);  // Set the radius of the ball
        circ.setPosition(x, y);  // Set the initial position of the ball
        speed = 200.f;  // Set the speed of the ball
        velocity = { -speed, -speed };  // Initialize the ball's movement direction (moving diagonally)

        // Load the texture from a file and apply it to the ball's shape
        if (!texture.loadFromFile(textureFile)) {
            std::cerr << "Error loading ball texture" << std::endl;  // Display an error if the texture fails to load
        }
        circ.setTexture(&texture);  // Apply the texture to the ball
    }

    // Move the ball based on its velocity and the time passed since the last frame (deltaTime)
    void move(float deltaTime) {
        circ.move(velocity * deltaTime);  // Update the ball's position based on its velocity
    }

    // Reset the ball's position and velocity after a point is scored
    void reset(float x, float y) {
        circ.setPosition(x, y);  // Reset ball to center of window
        velocity = { -speed, -speed };  // Reset the ball's velocity to its initial diagonal movement
    }

    // Check if the ball hits the top or bottom wall and reverse its vertical direction
    void checkCollisionWithWalls(float windowHeight) {
        if (circ.getPosition().y <= 0 || circ.getPosition().y >= windowHeight - circ.getRadius() * 2) {
            velocity.y = -velocity.y;  // Reverse the vertical velocity when hitting the top or bottom of the window
        }
    }

    // Check if the ball collides with a paddle and reverse its horizontal direction
    void checkCollisionWithPaddle(const sf::RectangleShape& paddle) {
        if (circ.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            velocity.x = -velocity.x;  // Reverse the horizontal velocity if it collides with a paddle
        }
    }

    // Check if the ball is out of bounds on either on the left or right side of the screen (x-axis)
    bool isOutOfBounds(float windowWidth) {
        return circ.getPosition().x <= 0 || circ.getPosition().x >= windowWidth;  // Return true if the ball is outside the window horizontally.
    }
};

class Game {
public:
    sf::RenderWindow window;
    Paddle leftPaddle;
    Paddle rightPaddle;
    Ball ball;
    int leftScore;
    int rightScore;
    const int scoreLimit = 5;
    bool leftPaddleMovingUp;

    // Font and text for displaying the score
    sf::Font font;
    sf::Text leftScoreText;
    sf::Text rightScoreText;
    
    sf::Text winnerText;
    
    bool gameOver; // Flag to indicate if the game is over
    sf::Clock gameOverClock; // Clock to measure the time after the game ends

    std::vector<sf::RectangleShape> dashedLine; //vector to hold dashes

    
    
    // contructor that initializes the game
    Game()
        : window(sf::VideoMode(800, 600), "Pong"),
          leftPaddle(50.f, 300.f - 50.f, "/Users/jbon/FinalProject/6010-Final-Project/paddle_texture.jpeg"),
          rightPaddle(750.f - 25.f, 300.f - 50.f, "/Users/jbon/FinalProject/6010-Final-Project/paddle_texture.jpeg"),
          ball(400.f, 300.f, "/Users/jbon/FinalProject/6010-Final-Project/fireball_texture.jpeg"),
          leftScore(0), rightScore(0),
          leftPaddleMovingUp(true),
          
          gameOver(false){ // Set gameOver to false initially
          window.setFramerateLimit(60);

        // Load font
        if(!font.loadFromFile("/Users/jbon/FinalProject/6010-Final-Project/BUBBLEGUMS.TTF")) {
            std::cerr << "Error loading font" << std::endl;
        }

          // Set up the left player's score text (font, size, color, position)
          leftScoreText.setFont(font);
          leftScoreText.setCharacterSize(30);
          leftScoreText.setFillColor(sf::Color::White);
          leftScoreText.setPosition(200.f, 20.f);  // Position it near the top left corner.

          // Set up the right player's score text (font, size, color, position)
          rightScoreText.setFont(font);
          rightScoreText.setCharacterSize(30);
          rightScoreText.setFillColor(sf::Color::White);
          rightScoreText.setPosition(600.f, 20.f);  // Position it near the top right corner.
          
              
          // Set up the winner message text
          winnerText.setFont(font);
          winnerText.setCharacterSize(30);
          winnerText.setFillColor(sf::Color::Yellow);
          winnerText.setPosition(200.f, 300.f);  // Center the message
              
          updateScoreText();  // Initialize the score text to display the current score

          // Move dashed line creation logic into the constructor:
          float dashHeight = 10.f;  // Height of each dash.
          float dashWidth = 5.f;    // Width of each dash.
          float gap = 10.f;         // Gap between each dash.
          float xPosition = (window.getSize().x / 2) - (dashWidth / 2);  // Calculate the X position to center the dashes.

          for (float yPosition = 0; yPosition < window.getSize().y; yPosition += dashHeight + gap) {
              sf::RectangleShape dash(sf::Vector2f(dashWidth, dashHeight));  // Create a rectangle to represent a dash.
              dash.setPosition(xPosition, yPosition);  // Set the position of the dash in the middle of the screen.
              dash.setFillColor(sf::Color::White);  // Set the dash color to white.
              dashedLine.push_back(dash);  // Add the dash to the dashedLine vector.
          }
      }


    void run() {
        
        sf::Clock clock;  // Clock to measure deltaTime (time passed since last frame)

        while (window.isOpen()) {  // Keep running while the window is open
            float deltaTime = clock.restart().asSeconds();  // Measure time passed since the last frame
            // Handle both paddles movement based on user input
            sf::Event event;
            while(window.pollEvent(event)){
                
                if (event.type == sf::Event::Closed) { // If the window is closed
                    window.close();  // Close the window and stop the game.
                }
                
                if(!gameOver) {
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up ) {
                        rightPaddle.moveUp(deltaTime);  // Move the right paddle up when the up arrow is pressed
                    }
                    
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                        rightPaddle.moveDown(deltaTime, window.getSize().y);  // Move the right paddle down when the up arrow is pressed.
                    }
                    
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W ) {
                        leftPaddle.moveUp(deltaTime);  // Move the left paddle up when W is pressed.
                    }
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S ) {
                        leftPaddle.moveDown(deltaTime, window.getSize().y);  // Move the left paddle down when S is pressed.
                    }
                    
                }
            }
        if (!gameOver) {  // Only update the game if it's not over
            
            update(deltaTime);  // Update the game state
        }else {
           // If the game is over, display the winner message for 3 seconds
           if (gameOverClock.getElapsedTime().asSeconds() > 3.f) {
               window.close();  // Close the window after 3 seconds
           }
        }
           
            render();  // Render all the objects on the window
        }
    }

    
private:
 
    void update(float deltaTime) {
        
        ball.move(deltaTime);  // Move the ball.

        ball.checkCollisionWithWalls(window.getSize().y);  // Check for collisions between the ball and the top/bottom walls.
        ball.checkCollisionWithPaddle(leftPaddle.rec);   // Check for collisions between the ball and the left paddle.
        ball.checkCollisionWithPaddle(rightPaddle.rec);  // Check for collisions between the ball and the right paddle.

        
        // Check if the ball is out of bounds and update the score.
        if (ball.isOutOfBounds(window.getSize().x)) {
            if (ball.circ.getPosition().x <= 0) {  // If the ball goes out on the left (right player scores):
                rightScore++;  // Increase right player's score.
                leftPaddle.shrink();  // Shrink the left paddle.
                updateScoreText();  // Update the score display.
                checkWin();  // Check if someone has won the game.
                
                
            } else if (ball.circ.getPosition().x >= window.getSize().x - ball.circ.getRadius() * 2) {  // If the ball goes out on the right (left player scores):
                leftScore++;  // Increase left player's score.
                rightPaddle.shrink();  // Shrink the right paddle.
                updateScoreText();  // Update the score display.
                checkWin();  // Check if someone has won the game.
            }
            
            
            ball.reset(400.f, 300.f);  // Reset the ball to the center of the screen.
        }
    }
            

    
    
    
    void updateScoreText() {
        leftScoreText.setString(std::to_string(leftScore));  // Convert leftScore to string and set it in leftScoreText.
        rightScoreText.setString(std::to_string(rightScore));  // Convert rightScore to string and set it in rightScoreText.
    }

    
    
    void checkWin() {
        if (leftScore >= scoreLimit) {  // If the left player reaches the score limit
            winnerText.setString("Left Player Wins! ");
            gameOver = true;
            gameOverClock.restart();
    
           // window.close();  // Close the game window
            
        } else if (rightScore >= scoreLimit) {  // If the right player reaches the score limit
            winnerText.setString("Right Player Wins! ");
            gameOver = true;
            gameOverClock.restart();
            
            //window.close();  // Close the game window
        }
    }

    
    
    
    void render() {
        window.clear(sf::Color::Black);  // Clear the window with a black background.

        // Draw the dashed line in the middle of the screen from vector created
        for (const auto& dash : dashedLine) {
            window.draw(dash);  // Draw each dash (rectangle) in the vector.
        }

        window.draw(leftPaddle.rec);   // Draw the left paddle.
        window.draw(rightPaddle.rec);  // Draw the right paddle.
        window.draw(ball.circ);         // Draw the ball.

        window.draw(leftScoreText);      // Draw the left player's score.
        window.draw(rightScoreText);     // Draw the right player's score.

        if (gameOver) {
            window.draw(winnerText);  // Draw the winner message when the game is over
        }
        window.display();  // Display everything rendered in the window.
    }
    
    
    
};


int main() {
    Game game;
    game.run();
    return 0;
}



