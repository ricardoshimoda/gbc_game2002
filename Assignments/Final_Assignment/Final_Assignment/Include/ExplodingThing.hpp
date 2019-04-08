#pragma once

#include <SFML\Graphics.hpp>
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "Entity.hpp"

class ExplodingThing : public Entity
{
public:
	enum Type
	{
		Spaceship,
		Asteroid,
		Explosion,
	};

public:
	ExplodingThing(Type type, const TextureHolder& textures, sf::Vector2f theSpeed);
	sf::Sprite& getSprite();
	void setSprite(sf::Sprite sprite);
	virtual	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Vector2f getSpeed();
	void setSpeed(sf::Vector2f theSpeed);
	void setExpAnimIndex(int index);
	int getExpAnimIndex();

private:
	Type mType;
	sf::Sprite  mSprite;
	sf::Vector2f speed;
	int expAnimIndex;
};

