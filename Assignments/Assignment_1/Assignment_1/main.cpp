#include "include\main.hpp"


Game::Game()
	: mWindow(sf::VideoMode(640, 480), "SFML Application")
	, mPlayer()
	, texture()
	, background()
{

	mPlayer.setRadius(40.0f);
	mPlayer.setPosition(100.f, 100.f);
	mPlayer.setFillColor(sf::Color::Cyan);
	mPlayer.setPointCount(8);
	mPlayer.setOutlineThickness(10.0f);
	mPlayer.setOutlineColor(sf::Color(250, 150, 100));
	if (!texture.loadFromFile("Media/Textures/Hello.PNG")) {
		cout << "Cannot find the texture";
	}
	else {
		texture.setRepeated(true);
		background.setTexture(texture);
		background.setTextureRect(sf::IntRect(0, 0, 800, 600));
	}
}

void Game::run()
{

	while (mWindow.isOpen())
	{
		processEvents();
		update();
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::update()
{
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw(background);
	mWindow.draw(mPlayer);
	mWindow.display();
}

int main()
{

	Game game;
	game.run();
}


