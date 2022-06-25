#include <SFML/Graphics.hpp>
#include "Pathfinder.h"
#include "Menu.h"
#include <iostream>
using namespace std;

void start_the_game(sf::String playerInput, char flag, 
	static const unsigned int WIDTH, static const unsigned int HEIGHT);

int main()
{
	// screen resolution
	static const unsigned int WIDTH = 800;
	static const unsigned int HEIGHT = 600;

	sf::RenderWindow menu_window(sf::VideoMode(WIDTH, HEIGHT), "MAIN MENU");
	menu_window.setFramerateLimit(60);
	Menu menu(&menu_window, WIDTH, HEIGHT);

	sf::String playerInput;

	while (menu_window.isOpen())
	{
		sf::Event event;
		while (menu_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				menu_window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{			
					start_the_game(playerInput, '1', WIDTH, HEIGHT);
					playerInput.clear();
				}
				break;
			case sf::Event::TextEntered:
				menu.set_playerInput(playerInput += event.text.unicode);
				break;
			}
		}
		menu_window.clear();
		menu.draw();
		menu_window.display();
	}
	return 0;
}

void start_the_game(sf::String playerInput, char flag, 
	static const unsigned int WIDTH, static const unsigned int HEIGHT)
{
	//START THE GAME
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "THIS IS PATHFINDER!");
	window.setFramerateLimit(60);
	Pathfinder game(&window, WIDTH, HEIGHT, flag, playerInput);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window.close();
				}			
				break;
			}			
		}
		window.clear();
		game.draw();
		window.display();
	}
}
