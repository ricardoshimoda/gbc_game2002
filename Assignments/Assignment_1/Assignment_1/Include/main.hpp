#ifndef BOOK_GAME_HPP
#define BOOK_GAME_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class Game : private sf::NonCopyable
{
public:
	Game();
	void					run();


private:
	void					processEvents();
	void					update();
	void					render();


private:

	sf::RenderWindow		mWindow;
	sf::CircleShape				mPlayer;
	sf::Texture texture;
	sf::Sprite background;
};

#endif // BOOK_GAME_HPP