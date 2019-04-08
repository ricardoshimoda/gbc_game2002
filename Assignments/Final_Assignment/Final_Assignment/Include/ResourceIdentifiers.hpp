#pragma once
#pragma region step 5

namespace sf
{
	class Texture;
}

namespace Textures
{
	enum ID
	{
		Background,
		Spaceship,
		Asteroid,
		Explosion,
		Title,
		Play,
		Quit,
		BackgroundPause,
		PauseTitle,
		MainMenu,
		Resume,
		GameOverTitle,
		Retry,

	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;

#pragma endregion