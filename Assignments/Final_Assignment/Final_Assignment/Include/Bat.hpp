#ifndef BAT_HPP
#define BAT_HPP
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class Bat : private sf::NonCopyable
{
public:
	Bat();
	sf::Vector2f			getPosition();
	sf::RectangleShape		getShape();
	void					moveRight();
	void					moveLeft();
	void					stop();
	void					update();

private:
	static const float		maxSpeed;
	float					batSpeed;
	sf::RectangleShape      batShape;
	sf::Vector2f			position;
};

#endif // BAT_HPP

