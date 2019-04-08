#include "Include/ExplodingThing.hpp"

Textures::ID toTextureID(ExplodingThing::Type type)
{
	switch (type)
	{
	case ExplodingThing::Spaceship:
		return Textures::Spaceship;
	case ExplodingThing::Asteroid:
		return Textures::Asteroid;
	case ExplodingThing::Explosion:
		return Textures::Explosion;
	}
	return Textures::Spaceship;
}

ExplodingThing::ExplodingThing(Type type, const TextureHolder& textures, sf::Vector2f theSpeed)
{

	//connecting entities with resources

	mType = type;
	mSprite.setTexture(textures.get(toTextureID(type)));
	sf::FloatRect bounds = mSprite.getLocalBounds();
	//by default, the origin of sprites is located in their upper-left corner
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	//mSprite.setPosition(200, 200);
	speed = theSpeed;
	expAnimIndex = 0;
}

sf::Sprite& ExplodingThing::getSprite()
{
	return mSprite;
}

void ExplodingThing::setSprite(sf::Sprite sprite)
{
	mSprite = sprite;
}

void ExplodingThing::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

sf::Vector2f ExplodingThing::getSpeed()
{
	return speed;
}

void ExplodingThing::setSpeed(sf::Vector2f theSpeed)
{
	speed = theSpeed;
}

void ExplodingThing::setExpAnimIndex(int index)
{
	expAnimIndex = index;
}

int ExplodingThing::getExpAnimIndex()
{
	return expAnimIndex;
}
