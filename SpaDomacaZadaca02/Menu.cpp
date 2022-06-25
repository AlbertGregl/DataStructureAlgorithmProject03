#include "Menu.h"
#include <iostream>

Menu::Menu(sf::RenderWindow* menu_window, int screen_width, int screen_height)
{
	this->menu_window = menu_window;
	this->screen_width = screen_width;
	this->screen_height = screen_height;
	// load keyboard texture Esc (32x64)
	sf::Texture key_esc;
	if (!key_esc.loadFromFile("Esc-Key.png"))
	{
		std::cout << "Something went wrong while opening the file: \"Esc-Key.png\"" << std::endl;
	}
	this->key_esc = key_esc;
	// load font file
	if (!font.loadFromFile("dogicapixel.ttf"))
	{
		std::cout << "Something went wrong while opening the file: \"dogicapixel.ttf\"" << std::endl;
	}
	this->font = font;
}

void Menu::draw()
{
	// create background color
	sf::ConvexShape back;
	back.setPointCount(8);
	back.setPoint(0, sf::Vector2f(0.f, 0.f));
	back.setPoint(1, sf::Vector2f(screen_width, 0.f));
	back.setPoint(2, sf::Vector2f(screen_width, screen_height));
	back.setPoint(3, sf::Vector2f(0.f, screen_height));
	back.setFillColor(sf::Color(58, 59, 139));
	menu_window->draw(back);

	// heading
	sf::Text heading;
	heading.setFont(font);
	heading.setFillColor(sf::Color::White);
	heading.setString("Pathfinder");
	heading.setStyle(sf::Text::Bold);
	heading.setCharacterSize(64.f);
	heading.setPosition(120.f, 32.f);
	menu_window->draw(heading);
	
	// menu no.1 
	sf::Text menu1;
	menu1.setFont(font);
	menu1.setFillColor(sf::Color::White);
	menu1.setString("Enter starting and final");
	menu1.setPosition(32.f, 192.f);
	menu_window->draw(menu1);

	// menu no.2
	sf::Text menu2;
	menu2.setFont(font);
	menu2.setFillColor(sf::Color::White);
	menu2.setString("coordinates: ");
	menu2.setPosition(64.f, 256.f);
	menu_window->draw(menu2);

	// menu no.3
	sf::Text menu3;
	menu3.setFont(font);
	menu3.setFillColor(sf::Color::White);
	menu3.setString("... then press (ENTER).");
	menu3.setPosition(64.f, 320.f);
	menu_window->draw(menu3);

	// menu no.4
	sf::Text menu4;
	menu4.setFont(font);
	menu4.setFillColor(sf::Color::Cyan);
	menu4.setScale(0.5f, 0.5f);
	menu4.setString("format: 0,0-29,39");
	menu4.setPosition(384.f, 288.f);
	menu_window->draw(menu4);


	// menu no.5
	sf::Text menu5;
	menu5.setFont(font);
	menu5.setFillColor(sf::Color::Red);
	menu5.setString("Exit");
	menu5.setPosition(210.f, 448.f);
	menu_window->draw(menu5);
	// menu no.5 sprite
	sf::Sprite m5_sprite;
	m5_sprite.setTexture(key_esc);
	m5_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	m5_sprite.setPosition(128.f, 448.f);
	menu_window->draw(m5_sprite);

	menu_window->draw(playerText);
}

void Menu::set_playerInput(sf::String playerInput)
{	
	playerText.setFont(font);
	playerText.setFillColor(sf::Color::Green);
	playerText.setPosition(384.f, 256.f);
	playerText.setString(playerInput);
}

