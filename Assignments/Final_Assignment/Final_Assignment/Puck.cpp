#include "include\Puck.hpp"

Puck::Puck()
	:xVelocity(8)
	,yVelocity(6)
	,position(200.f, 200.f)
{
	if (!puckTexture.loadFromFile("Media/Textures/puck.png"))
	{
		std::cout << "Error: could not load puck file" << std::endl;
	}

	puckShape.setTexture(puckTexture);
	puckShape.setPosition(position);

}

sf::Vector2f Puck::getPosition()
{
	return position;
}

sf::Sprite Puck::getShape()
{
	return puckShape;
}

sf::Vector2f Puck::getVelocity()
{
	return sf::Vector2f(xVelocity, yVelocity);
}

void Puck::hitBottom()
{
	/*
	 * goes back
	 */
	position.x = 200.f;
	position.y = 200.f;
	puckShape.setPosition(position);
}

void Puck::reboundBatOrTop()
{
	yVelocity = -yVelocity; /* perfectly elastic shock */
}

void Puck::reboundSides()
{
	xVelocity = -xVelocity; /* again, perfectly elastic shock */
}

void Puck::update()
{
	position.x += xVelocity;
	position.y += yVelocity;
	puckShape.setPosition(position);
}
