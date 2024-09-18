//
//  main.cpp
//  pong
//
//  Created by Jose Bonilla on 9/17/24.
//

#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Window.hpp>
#include <iostream>
#include <memory>

const float paddleWidth = 25.f;
const float paddleHeight = 100.f;
const float ballRadius = 10.f;
const float paddleSpeed = 400.f;
//const float ballSpeed = 300.f;

int main() {
    // Create a game window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

    // Set the frame rate limit
    window.setFramerateLimit(60);

    // Define paddles and ball
    sf::RectangleShape leftPaddle(sf::Vector2f(paddleWidth, paddleHeight));
    sf::RectangleShape rightPaddle(sf::Vector2f(paddleWidth, paddleHeight));
    sf::CircleShape ball(ballRadius);
    
    

    // Set paddles and ball initial positions
    leftPaddle.setPosition(50.f, 300.f - paddleHeight / 2);
    rightPaddle.setPosition(750.f - paddleWidth, 300.f - paddleHeight / 2);
    ball.setPosition(400.f, 300.f);
    
    float xstep = 4;
    float ystep = 4;

    
    
    sf::Sprite player;
    player.setRotation(90);
    //player.setTexture(500, 200);
    player.setPosition(400.0f, 500.0f);  // Start player at a position

    // Main game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
        }
        

        ball.move(xstep, ystep);
        if (ball.getPosition().x > 800-20){
            
            xstep = -xstep; //change the direction
            
        }
        else if (ball.getPosition().x < 0 + 20){
            
            xstep = -xstep;
        }
        
        ball.move(xstep, ystep);
        if (ball.getPosition().y > 600-20){
            
            ystep = -ystep; //change the direction
            
        }
        else if (ball.getPosition().y < 0 + 20){
            
            ystep = -ystep;
        }
        // Clear the screen
        window.clear(sf::Color::Black);

        // Draw paddles and ball
        window.draw(leftPaddle);
        window.draw(rightPaddle);
        window.draw(ball);

        // Display everything
        window.display();
    }

    return 0;
}




