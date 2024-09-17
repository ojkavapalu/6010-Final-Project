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

/*
class paddle{
    
    float paddleWidth = 25.f;
    float paddleHeight = 100.f;
    const float paddleSpeed = 400.f;
    
    

    
    
    
    
    
    
    
    
    
};



class ball{
    const float ballRadius = 10.f;
    const float ballSpeed = 300.f;
    
    
};




class game{

public:
    
    //creates window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");
    
    //sets widown frames
    window.setFramerateLimit(60);
    
    sf::RectangleShape leftPaddle(sf::Vector2f(paddleWidth, paddleHeight));
    sf::RectangleShape rightPaddle(sf::Vector2f(paddleWidth, paddleHeight));
    sf::CircleShape ball(ballRadius);
    
    
    
    
    
    
    
}


int main() {



    // Set paddles and ball initial positions
    leftPaddle.setPosition(50.f, 300.f - paddleHeight / 2);
    rightPaddle.setPosition(750.f - paddleWidth, 300.f - paddleHeight / 2);
    ball.setPosition(400.f, 300.f);

    // Initialize velocity for ball movement
    sf::Vector2f ballVelocity(-ballSpeed, -ballSpeed);

    // Main game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
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

*/


const float paddleWidth = 25.f;
const float paddleHeight = 100.f;
const float ballRadius = 10.f;
const float paddleSpeed = 400.f;
const float ballSpeed = 300.f;

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
    ball.setPosition(780.f, 1.f);

    // Initialize velocity for ball movement
    sf::Vector2f ballVelocity(-ballSpeed, -ballSpeed);
    
    
    

    // Main game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
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

