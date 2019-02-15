#ifndef PUCK_HPP
#define PUCK_HPP
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class Puck : private sf::NonCopyable
{
public:
	Puck();
	sf::Vector2f			getPosition();
	sf::Sprite				getShape();
	sf::Vector2f			getVelocity();
	void					hitBottom();
	void					reboundBatOrTop();
	void					reboundSides();
	void					update();

private:
	sf::Texture				puckTexture;
	sf::Sprite              puckShape;
	float					xVelocity;
	float					yVelocity;
	sf::Vector2f			position;
};
#endif // PUCK_HPP
