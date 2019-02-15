#include "include\Game.hpp"


/*
 * this makes the time per frame constant so that we avoid unwanted results
 */
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
/* And we start the "in-line" initializers  */
	: mWindow(sf::VideoMode(800, 800), "SFML Application")
	, mView(mWindow.getDefaultView())
	, mWorldBounds(0.f, 0.f, mView.getSize().x, mView.getSize().y)
	, background()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, puck()
	, bat()
	, mScoreText()
	, playerLives(5)
	, playerScore(0)
	, mIsMovingLeft(false)
	, mIsMovingRight(false)
	, mIsColliding(false)
	, mPlayerCanMove(true)
	, mIcon()
	, mMusic()
{
	/*
	 */
	mIcon.loadFromFile("Media/Textures/tml.png");
	mWindow.setIcon(mIcon.getSize().x, mIcon.getSize().y, mIcon.getPixelsPtr());
	
	if (!backTexture.loadFromFile("Media/Textures/ice.jpg"))
	{
		std::cout << "Error: could not load background file" << std::endl;
	}
	/*
	 * Apply background texture to the window
	 */
	backTexture.setRepeated(true);
	background.setTexture(backTexture);
	background.setTextureRect(sf::IntRect(mWorldBounds));


	/*
	 * Set statistics text properties
	 */
	if (!mFont.loadFromFile("Media/Fonts/electroharmonix.ttf")) {
		std::cout << "Error: could not load statistics font file" << std::endl;
	}
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, mView.getSize().y - 40.f);
	mStatisticsText.setCharacterSize(15);
	mStatisticsText.setFillColor(sf::Color::Black);

	/*
	 * Set Score text properties
	 */
	if (!mScoreFont.loadFromFile("Media/Fonts/score.ttf")) {
		std::cout << "Error: could not load score font file" << std::endl;
	}
	mScoreText.setFont(mScoreFont);
	mScoreText.setPosition(5.f, 5.f);
	mScoreText.setCharacterSize(30);
	mScoreText.setFillColor(sf::Color::Black);
	updateScore(); // first call outside loop

	/*
	 * Create the End Game objects 
	 */
	if (!mEndGameFont.loadFromFile("Media/Fonts/ZombieSlayer.ttf")) {
		std::cout << "Error: could not load end game font file" << std::endl;
	}
	mEndGameText.setFont(mEndGameFont);
	mEndGameText.setPosition(175.f, 300.f);
	mEndGameText.setCharacterSize(60);
	mEndGameText.setFillColor(sf::Color::Red);
	mEndGameText.setString("This is the end");

	/*
	 * Music, please
	 */
	//mMusic.openFromFile("Media/Sounds/dannythedog.wav");
	//mMusic.openFromFile("Media/Textures/Tchaikovsky_Nocturne__orch.mp3");  //does not play MP3 - license
	//Play the music
	//mMusic.setAttenuation(100.f);
	//mMusic.play();
	//mMusic.setLoop(true);

}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + to_string(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Game::updateScore()
{
	mScoreText.setString("Score:" + to_string(playerScore) + "                Lives:" + to_string(playerLives));
}


void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		processEvents();
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update();
		}

		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}


void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::A)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;

	if (key == sf::Keyboard::Left)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::Right)
		mIsMovingRight = isPressed;
}


void Game::update()
{
	if (mPlayerCanMove) {
		if (mIsMovingLeft)
			bat.moveLeft();
		if (mIsMovingRight)
			bat.moveRight();
		if (!mIsMovingLeft && !mIsMovingRight)
			bat.stop();

		bat.update();
		puck.update();

	}

	float puckX = puck.getPosition().x;
	float puckY = puck.getPosition().y;
	float puckSize = puck.getShape().getTexture()->getSize().x;

	float batX = bat.getPosition().x;
	float batY = bat.getPosition().y;
	float batSizeX = bat.getShape().getSize().x;
	float batSizeY = bat.getShape().getSize().y;

	/*
	 * bat and puck connected
	 */
	if ((puckX + puckSize > batX && puckX < batX + batSizeX) /* there is an horizontal connection */
		&& (puckY + puckSize > batY && puckY  + puckSize < batY + batSizeY/2) /* there is an horizontal connection */
		&& !mIsColliding
		){
		playerScore++;
		updateScore(); // call only when necessary
		puck.reboundBatOrTop();
		mIsColliding = true;
	}
	else {
		mIsColliding = false;
	}

	/*
	 * bounds
	 */
	if (puckX <= 0 || puckX >= mView.getSize().x - puckSize) {
		puck.reboundSides();
	}
	if (puckY <= 0) {
		puck.reboundBatOrTop();
	}
	else if (puckY >= mView.getSize().y - puckSize) {
		playerLives--;
		updateScore(); // call only when necessary
		puck.hitBottom();
		if (playerLives == 0) {
			mPlayerCanMove = false;
		}
	}

	/*
	 * bat
	 */

}

void Game::render()
{
	mWindow.clear();
	mWindow.draw(background);
	if (mPlayerCanMove) {

		mWindow.draw(puck.getShape());
		mWindow.draw(bat.getShape());
	}
	else {
		mWindow.draw(mEndGameText);
	}
	mWindow.draw(mStatisticsText);
	mWindow.draw(mScoreText);
	mWindow.display();
}

