#include "include\Bat.hpp"

const float	Bat::maxSpeed = 20;

Bat::Bat()
	:batShape()
	, batSpeed(0)
	, position(220.f, 700.f)
{
	batShape.setPosition(position);
	batShape.setSize(sf::Vector2f(180,30));
	batShape.setFillColor(sf::Color::Black);
}

sf::Vector2f Bat::getPosition()
{
	return position;
}

sf::RectangleShape Bat::getShape()
{
	return batShape;
}

void Bat::moveRight()
{
	/* toujours */
	batSpeed = maxSpeed;
}

void Bat::moveLeft()
{
	/* a toute vitesse */
	batSpeed = -maxSpeed;
}

void Bat::stop()
{
	batSpeed = 0;
}

void Bat::update()
{
	position.x += batSpeed;
	if (position.x <= 0)
		position.x = 0;
	if (position.x >= 800 - 180)
		position.x = 800.f - 180.f;
	batShape.setPosition(position);

}
