#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "SFML/Graphics.hpp"


class UIElement : public sf::Drawable, public sf::Transformable

{
public:
	enum UIType
	{
		Title,
		Play,
		Quit,
		GameOverTitle,
		PauseTitle,
		Retry,
		MainMenu,
		Resume,
	};

public:
	UIElement(UIType type, const TextureHolder& textures);
	sf::Sprite&		getSprite();
	void			setSprite(sf::Sprite sprite);
	virtual	void	drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void			setExpAnimIndex(int index);
	int				getExpAnimIndex();
	void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float			getWidth();
	float			getHeight();

private:
	UIType mType;
	sf::Sprite  mSprite;
	sf::Vector2f speed;
	int expAnimIndex;
	sf::FloatRect bounds;

};


