#ifndef GAME_HPP
#define GAME_HPP


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <iostream>
#include <vector>

#include "Include/ResourceHolder.hpp"
#include "Include/ResourceIdentifiers.hpp"

#include "Include/ExplodingThing.hpp"
#include "Include/UIElement.hpp"



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
	void					mainMenu();
	void					animateButton();
	void					initializeGame();
	void					movePlayer();
	void					animatePlayer();
	void					shoot();
	void					moveProjectiles();
	void					animateProjectiles();
	void					spawnAsteroids();
	void					moveAsteroids();
	sf::Vector2f			calculateAsteroidDirection(sf::Vector2f position);
	void					animateAsteroids();
	void					collisionBulletAsteroid();
	void					addExplosion(sf::Vector2f position);
	void					animateExplosions();
	void					collisionPlayerAsteroid();
	void					pauseMenu();
	void					gameOver();

private:
	/* Player Speed */
	static const float PlayerSpeed;
	/* movement independent of frames/second + statistics */
	static const sf::Time	TimePerFrame;
	sf::Font				mFont;
	std::size_t				mStatisticsNumFrames;
	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;

	sf::RenderWindow		mWindow;
	sf::View				mView;
	sf::FloatRect			mWorldBounds;

	/* Game UI elements */
	sf::Sprite				background, 
		                    backgroundPause, 
		                    pauseTitle, 
		                    resume, 
		                    mainMenuPause, 
		                    gameOverTitle, 
		                    retry, 
		                    mainMenuGameOver;

	/* replacing all texture by this texture holder */
	TextureHolder			mTextures;

	/*
	 * Player related data
	 */
	ExplodingThing			*mPlayer;
	vector<ExplodingThing*> projectiles;
	vector<ExplodingThing*> asteroids;
	vector<ExplodingThing*> explosions;
	sf::Sprite				playerForward;

	UIElement				*mTitle, *mPlay, *mQuit, *mGameOverTitle, *mPauseTitle, *mRetry, *mMainMenu, *mResume;

	/*
	 * player related data
	 */
	sf::Font				mScoreFont;
	sf::Text				mScoreText;
	int						playerLives;
	int						playerScore;

	/*
	 * player movement
	 */
	bool					mIsMovingLeft, mIsMovingRight, mIsMovingUp, mIsMovingDown, mIsColliding;

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
