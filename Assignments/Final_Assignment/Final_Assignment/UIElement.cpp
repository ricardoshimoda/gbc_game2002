#include "Include/UIElement.hpp"

Textures::ID toTextureID(UIElement::UIType type)
{
	switch (type)
	{
	case UIElement::Play:
		return Textures::Play;
	case UIElement::Quit:
		return Textures::Quit;
	case UIElement::Title:
		return Textures::Title;
	case UIElement::GameOverTitle:
		return Textures::GameOverTitle;
	case UIElement::PauseTitle:
		return Textures::PauseTitle;
	case UIElement::Retry:
		return Textures::Retry;
	case UIElement::MainMenu:
		return Textures::MainMenu;
	case UIElement::Resume:
		return Textures::Resume;

	}
	return Textures::Title;
}



UIElement::UIElement(UIType type, const TextureHolder& textures)
{
	mType = type;
	mSprite.setTexture(textures.get(toTextureID(type)));
	bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	expAnimIndex = 0;
}

float UIElement::getWidth() {
	return bounds.width;
}
float UIElement::getHeight() {
	return bounds.width;

}

void UIElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw node with changed transform
	drawCurrent(target, states);
}


void UIElement::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}


sf::Sprite& UIElement::getSprite()
{
	return mSprite;
}

void UIElement::setSprite(sf::Sprite sprite)
{
	mSprite = sprite;
}


void UIElement::setExpAnimIndex(int index)
{
	expAnimIndex = index;
}

int UIElement::getExpAnimIndex()
{
	return expAnimIndex;
}
