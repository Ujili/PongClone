#include "Bat.h"
#include "Ball.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>

int main()
{
	//Create a video mode object
	VideoMode vm(1920, 1080);

	//Create and open a window for the game
	RenderWindow window(vm, "Pong", Style::Fullscreen);

	//Set beginning variables
	int score = 0;
	int lives = 3;

	//Create a bat for the player; in the middle, about 20 pixs up
	Bat bat(1920 / 2, 1080 - 20);

	//Create the ball
	Ball ball(1920 / 2, 0);

	//Create a text object for the HUB
	Text hud;

	//Get the retro font
	Font pongFont;
	pongFont.loadFromFile("fonts/8bitOperatorPlus8-Regular.ttf");

	//Set the font to our HUD
	hud.setFont(pongFont);

	//Size it as necessary
	hud.setCharacterSize(75);

	//Give it a color to stand out
	hud.setFillColor(Color::Green);

	//Set the HUD position; adjust as needed
	hud.setPosition(20, 20);

	//Let's create the update clock
	Clock clock;

	while (window.isOpen())
	{
		/*
		Handle the Player input
		*/
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				//Quit the game if the window is closed
				window.close();
		}

		//Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//Handle the pressing and releasing of the left and right arrow keys
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			bat.moveLeft();
		}
		else
		{
			bat.stopLeft();
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			bat.moveRight();
		}
		else
		{
			bat.stopRight();
		}

		/*
		Update the bat, ball, and the HUD
		*/
		//Update the delta time
		Time dt = clock.restart();
		bat.update(dt);
		ball.update(dt);
		//Update the HUD text
		std::stringstream ss;
		ss << "Score: " << score << " Lives: " << lives;
		hud.setString(ss.str());

		//Handle the ball hitting the bottom
		if (ball.getPosition().top > window.getSize().y)
		{
			//reverse the ball direction
			ball.reboundBottom();

			//Remove a life
			lives--;

			//Check if zero lives
			if (lives < 1)
			{
				//reset the score
				score = 0;
				//reset the lives
				lives = 3;
			}
		}

		//Handle the ball hitting the top
		if (ball.getPosition().top < 0)
		{
			ball.reboundBatOrTop();

		}

		//Handle the ball hitting sides
		if (ball.getPosition().left < 0 || ball.getPosition().left + ball.getPosition().width > window.getSize().x)
		{
			ball.reboundSides();
		}

		//Has the ball hit the bat?
		if (ball.getPosition().intersects(bat.getPosition()))
		{
			//Hit detected, so reverse the ball and score a point
			ball.reboundBatOrTop();
			score++;
		}

		/*
		Draw the bat, the ball, and the HUD
		*/
		window.clear();
		window.draw(hud);
		window.draw(bat.getShape());
		window.draw(ball.getShape());
		window.display();
	}

	return 0;
}