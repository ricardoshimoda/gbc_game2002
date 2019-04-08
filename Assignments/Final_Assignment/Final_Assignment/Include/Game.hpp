#ifndef GAME_HPP
#define GAME_HPP


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include "Include/Puck.hpp"
#include "Include/Bat.hpp"

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
	void					updateStatistics(sf::Time elapsedTime);
	void					updateScore();
	void					handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
	/* movement independent of frames/second + statistics */
	static const sf::Time	TimePerFrame;
	sf::Font				mFont;
	std::size_t				mStatisticsNumFrames;
	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;

	sf::RenderWindow		mWindow;
	sf::View				mView;
	sf::FloatRect			mWorldBounds;
	sf::Texture				backTexture;
	sf::Sprite				background;

	/*
	 * Puck related data
	 */
	Puck					puck;

	/*
	 * player related data
	 */
	sf::Font				mScoreFont;
	sf::Text				mScoreText;
	int						playerLives;
	int						playerScore;
	Bat						bat;

	/*
	 * player movement
	 */
	bool					mIsMovingLeft;
	bool					mIsMovingRight;
	bool					mIsColliding;

	/*
	 * EndGame
	 */
	bool					mPlayerCanMove;
	sf::Font				mEndGameFont;
	sf::Text				mEndGameText;

	/*
	 * decoration 
	 */
	sf::Image				mIcon;
	sf::Music				mMusic;


};
#endif // GAME_HPP
