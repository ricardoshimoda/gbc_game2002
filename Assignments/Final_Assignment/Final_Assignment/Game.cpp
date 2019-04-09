#include "include\Game.hpp"


/*
 * this makes the time per frame constant so that we avoid unwanted results
 */
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

int gameState = 0; // 0: Main Menu		1: Gameplay		2: Pause		3: Game Over

/*
 * Timers for all animations
 */
float playerTimer = 0, shootTimer = 0, bulletTimer = 0, asteroidTimer = 0, asteroidAnimationTimer = 0, expAnimTimer = 0, buttonTimer = 0, menuTimer = 0;
int asteroidAnimationIndex = 0;

/*
 * Control Animation States
 */
bool playerGas1 = true, canShoot = true, bulletTexture1 = true, drawButton = true, menuActive = true;

/*
 * Control Active UI Button
 */
string activeButton = "play";

/*
 * Speed of stuff
 */
const float Game::PlayerSpeed = 200.f, projectileSpeed = 300.f, asteroidSpeed = 250.f;

Game::Game()
/* And we start the "in-line" initializers  */
	: mWindow(sf::VideoMode(1280, 800), "Final Assignment - Ricardo Shimoda Nakasako - 101128885")
	, mView(mWindow.getDefaultView())
	, mWorldBounds(0.f, 0.f, mView.getSize().x, mView.getSize().y)
	, mTextures()
	, background()
	, mStatisticsText()
	, mBlankStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mPlayer()

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

	try
	{
		mTextures.load(Textures::Background, "Media/textures/background.jpg");
		mTextures.load(Textures::Spaceship, "Media/textures/spaceship.png");
		mTextures.load(Textures::Asteroid, "Media/textures/rock.png");
		mTextures.load(Textures::Explosion, "Media/textures/explosions/typeB.png");
		mTextures.load(Textures::Title, "Media/textures/UI/title.png");
		mTextures.load(Textures::Play, "Media/textures/UI/play.png");
		mTextures.load(Textures::Quit, "Media/textures/UI/quit.png");
		mTextures.load(Textures::BackgroundPause, "Media/textures/UI/backgroundPause.png");
		mTextures.load(Textures::PauseTitle, "Media/textures/UI/gamePaused.png");
		mTextures.load(Textures::MainMenu, "Media/textures/UI/mainMenu.png");
		mTextures.load(Textures::Resume, "Media/textures/UI/resume.png");
		mTextures.load(Textures::GameOverTitle, "Media/textures/UI/gameOver.png");
		mTextures.load(Textures::Retry, "Media/textures/UI/retry.png");
		mFont.loadFromFile("Media/Fonts/electroharmonix.ttf");
		mScoreFont.loadFromFile("Media/Fonts/score.ttf");
	}
	catch (std::runtime_error& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	/*
	 * Loads the icon for the application - just a Single Jet 
	 */
	mIcon.loadFromFile("Media/Textures/SingleJet.png");
	mWindow.setIcon(mIcon.getSize().x, mIcon.getSize().y, mIcon.getPixelsPtr());

	/*
	 * Loading all UI elements
	 */
	sf::Texture& texture = mTextures.get(Textures::Background);
	background.setTexture(texture);
	background.setTextureRect(sf::IntRect(mWorldBounds));
	background.setPosition(mWorldBounds.left, mWorldBounds.top);

	sf::Texture& pauseBackgroundTexture = mTextures.get(Textures::BackgroundPause);
	backgroundPause.setTexture(pauseBackgroundTexture);
	backgroundPause.setTextureRect(sf::IntRect(mWorldBounds));
	backgroundPause.setPosition(mWorldBounds.left, mWorldBounds.top);

	/*
	 * Main menu
	 */
	sf::Texture& mainMenuTexture = mTextures.get(Textures::MainMenu);
	mainMenuPause.setTexture(mainMenuTexture);
	mainMenuPause.setTextureRect(sf::IntRect(0, 0, 228, 32));
	mainMenuPause.setPosition(340, 400);

	mTitle = new UIElement(UIElement::Title, mTextures);
	mTitle->setPosition(398 / 2 + 10, 140 / 2 + 10);

	mPlay = new UIElement(UIElement::Play, mTextures);
	mPlay->setPosition(610, 300);

	mQuit = new UIElement(UIElement::Quit, mTextures);
	mQuit->setPosition(610, 400);

	mGameOverTitle = new UIElement(UIElement::GameOverTitle, mTextures);
	mGameOverTitle->setPosition((1280)/2, 200);

	mRetry = new UIElement(UIElement::Retry, mTextures);
	mRetry->setPosition((1280) / 2, 400);

	mMainMenu = new UIElement(UIElement::MainMenu, mTextures);
	mMainMenu->setPosition(1280 / 2, 500);

	mPauseTitle = new UIElement(UIElement::PauseTitle, mTextures);
	mPauseTitle->setPosition((1280) / 2, 200);

	mResume = new UIElement(UIElement::Resume, mTextures);
	mResume->setPosition((1280) / 2, 400);

	mPlayer = new ExplodingThing(ExplodingThing::Spaceship, mTextures, sf::Vector2f(PlayerSpeed, PlayerSpeed));
	mPlayer->setVelocity(0, 0);
	mPlayer->setPosition(430, 500);
	mPlayer->setSprite(playerForward);


	mBlankStatisticsText.setFont(mFont);
	mBlankStatisticsText.setPosition(5.f, mView.getSize().y - 40.f);
	mBlankStatisticsText.setCharacterSize(15);
	mBlankStatisticsText.setFillColor(sf::Color::White);

	/*
	 * Set statistics text properties
	 */
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, mView.getSize().y - 40.f);
	mStatisticsText.setCharacterSize(15);
	mStatisticsText.setFillColor(sf::Color::Black);

	/*
	 * Set Score text properties
	 */
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
		string updateStats =
			"Frames / Second = " + to_string(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us";
		
		mStatisticsText.setString(updateStats);

		mBlankStatisticsText.setString(updateStats);


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

	if (key == sf::Keyboard::W)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;

	if (key == sf::Keyboard::Left)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::Right)
		mIsMovingRight = isPressed;

	if (key == sf::Keyboard::Up)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::Down)
		mIsMovingDown = isPressed;
}


void Game::update()
{
	if (gameState == 0)
	{
		mainMenu();
		animateButton();
	}
	else if (gameState == 1)
	{
		movePlayer();
		animatePlayer();
		moveProjectiles();
		animateProjectiles();
		shoot();
		spawnAsteroids();
		moveAsteroids();
		animateAsteroids();
		animateExplosions();
		collisionBulletAsteroid();
		collisionPlayerAsteroid();
	}
	else if (gameState == 2)
	{
		animateButton();
		pauseMenu();
	}
	else
	{
		moveProjectiles();
		animateProjectiles();
		spawnAsteroids();
		moveAsteroids();
		animateAsteroids();
		animateExplosions();
		animateButton();
		gameOver();
	}
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw(background);
	if (gameState != 1) {
		mWindow.draw(backgroundPause);
	}

	if (gameState == 0)
	{
		mWindow.draw(*mTitle);
		if (activeButton != "play")
			mWindow.draw(*mPlay);
		else if (drawButton)
			mWindow.draw(*mPlay);
		if (activeButton != "quit")
			mWindow.draw(*mQuit);
		else if (drawButton)
			mWindow.draw(*mQuit);
	}
	if (gameState == 1 || gameState == 2 || gameState == 3)
	{
		mWindow.draw(*mPlayer);
		for (int i = 0; i < projectiles.size(); i++)
		{
			mWindow.draw(*projectiles[i]);
		}
		for (int i = 0; i < asteroids.size(); i++)
		{
			mWindow.draw(*asteroids[i]);
		}
		for (int i = 0; i < explosions.size(); i++)
		{
			mWindow.draw(*explosions[i]);
		}
	}
	if (gameState == 2)
	{
		mWindow.draw(*mPauseTitle);
		if (activeButton != "resume")
			mWindow.draw(*mResume);
		else if (drawButton)
			mWindow.draw(*mResume);
		if (activeButton != "mainMenuPause")
			mWindow.draw(*mMainMenu);
		else if (drawButton)
			mWindow.draw(*mMainMenu);
	}
	if (gameState == 3)
	{
		mWindow.draw(*mGameOverTitle);
		if (activeButton != "retry")
			mWindow.draw(*mRetry);
		else if (drawButton)
			mWindow.draw(*mRetry);
		if (activeButton != "mainMenuGameOver")
			mWindow.draw(*mMainMenu);
		else if (drawButton)
			mWindow.draw(*mMainMenu);
	}
	if (gameState != 1) {
		mWindow.draw(mStatisticsText);
	}
	else {
		mWindow.draw(mBlankStatisticsText);
	}
	mWindow.display();
}

void Game::animateButton()
{
	buttonTimer += TimePerFrame.asSeconds();
	if (buttonTimer >= 0.5f)
	{
		drawButton = !drawButton;
		buttonTimer = 0;
	}
}


void Game::mainMenu()
{
	menuTimer += TimePerFrame.asSeconds();
	if (menuTimer >= 0.5f)
	{
		menuActive = true;
		menuTimer = 0;
	}
	if (menuActive)
	{
		/*
		 * alternates between the two buttons accordingly
		 */
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			menuActive = false;
			if (activeButton == "play")
				activeButton = "quit";
			else
				activeButton = "play";
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && activeButton == "play")
		{
			menuActive = false;
			gameState = 1;
			initializeGame();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && activeButton == "quit")
		{
			menuActive = false;
			exit(0);
		}
	}
}

void Game::initializeGame()
{
	mPlayer->setPosition(430, 500);
	mPlayer->setSprite(playerForward);
	projectiles.clear();
	asteroids.clear();
	explosions.clear();
}

void Game::movePlayer()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		gameState = 2;
		activeButton = "resume";
	}
	sf::Vector2f movement(0.f, 0.f);
	sf::Vector2f position = mPlayer->getPosition();
	float playerTop = mPlayer->getPosition().y;
	float playerBottom = mPlayer->getPosition().y + mPlayer->getSprite().getGlobalBounds().height;
	float playerLeft = mPlayer->getPosition().x;
	float playerRight = mPlayer->getPosition().x + mPlayer->getSprite().getGlobalBounds().width;

	if (playerTop > mWorldBounds.top)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			mIsMovingUp = true;
			movement.y -= PlayerSpeed;
		}
		else
		{
			mIsMovingUp = false;
		}
	}
	if (playerBottom < mWorldBounds.top + mWorldBounds.height)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			mIsMovingDown = true;
			movement.y += PlayerSpeed;
		}
		else
		{
			mIsMovingDown = false;
		}
	}
	if (playerLeft > mWorldBounds.left)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			mIsMovingLeft = true;
			movement.x -= PlayerSpeed;
		}
		else
		{
			mIsMovingLeft = false;
		}
	}
	if (playerRight < mWorldBounds.left + mWorldBounds.width)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			mIsMovingRight = true;
			movement.x += PlayerSpeed;
		}
		else
		{
			mIsMovingRight = false;
		}
	}

	mPlayer->move(movement * TimePerFrame.asSeconds());
	mPlayer->update(TimePerFrame);
}

void Game::animatePlayer()
{
	playerTimer += TimePerFrame.asSeconds();

	if (playerTimer >= 0.2f)
	{
		playerGas1 = !playerGas1;
		playerTimer = 0;
	}

	sf::Texture& playerTexture = mTextures.get(Textures::Spaceship);
	sf::Sprite playerLeft(playerTexture, sf::IntRect(2, 0, 30, 36));
	sf::Sprite playerForward(playerTexture, sf::IntRect(39, 0, 39, 36));
	sf::Sprite playerRight(playerTexture, sf::IntRect(86, 0, 29, 36));
	sf::Sprite playerLeftGas1(playerTexture, sf::IntRect(2, 40, 30, 43));
	sf::Sprite playerForwardGas1(playerTexture, sf::IntRect(39, 40, 39, 43));
	sf::Sprite playerRightGas1(playerTexture, sf::IntRect(86, 40, 29, 43));
	sf::Sprite playerLeftGas2(playerTexture, sf::IntRect(2, 86, 30, 40));
	sf::Sprite playerForwardGas2(playerTexture, sf::IntRect(39, 86, 39, 40));
	sf::Sprite playerRightGas2(playerTexture, sf::IntRect(86, 86, 30, 40));

	if (mIsMovingUp && !mIsMovingDown && !mIsMovingLeft && !mIsMovingRight)
	{
		if (playerGas1)
			mPlayer->setSprite(playerForwardGas1);
		else
			mPlayer->setSprite(playerForwardGas2);
	}
	else if (mIsMovingUp && !mIsMovingDown && mIsMovingLeft && !mIsMovingRight)
	{
		if (playerGas1)
			mPlayer->setSprite(playerLeftGas1);
		else
			mPlayer->setSprite(playerLeftGas2);
	}
	else if (mIsMovingUp && !mIsMovingDown && !mIsMovingLeft && mIsMovingRight)
	{
		if (playerGas1)
			mPlayer->setSprite(playerRightGas1);
		else
			mPlayer->setSprite(playerRightGas2);
	}
	else if (!mIsMovingUp && mIsMovingLeft && !mIsMovingRight)
	{
		mPlayer->setSprite(playerLeft);
	}
	else if (!mIsMovingUp && !mIsMovingLeft && mIsMovingRight)
	{
		mPlayer->setSprite(playerRight);
	}
	else
	{
		mPlayer->setSprite(playerForward);
	}
}

void Game::shoot()
{
	if (!canShoot)
	{
		shootTimer += TimePerFrame.asSeconds();
	}
	if (shootTimer >= 0.5f)
	{
		canShoot = true;
		shootTimer = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canShoot)
	{
		sf::Texture& playerTexture = mTextures.get(Textures::Spaceship);
		sf::Sprite bullet1(playerTexture, sf::IntRect(56, 131, 4, 14));
		ExplodingThing* tempBullet(new ExplodingThing(ExplodingThing::Spaceship, mTextures, sf::Vector2f(0, -projectileSpeed)));
		tempBullet->setPosition(mPlayer->getPosition().x + 17, mPlayer->getPosition().y - 17);
		tempBullet->setSprite(bullet1);
		projectiles.push_back(tempBullet);
		canShoot = false;
	}
}

void Game::moveProjectiles()
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->move(projectiles[i]->getSpeed() * TimePerFrame.asSeconds());
		projectiles[i]->update(TimePerFrame);
		if (projectiles[i]->getPosition().y < -20)
		{
			projectiles.erase(projectiles.begin() + i);
		}
	}
}

void Game::animateProjectiles()
{
	bulletTimer += TimePerFrame.asSeconds();
	if (bulletTimer >= 0.1f)
	{
		sf::Texture& playerTexture = mTextures.get(Textures::Spaceship);
		sf::Sprite bullet1(playerTexture, sf::IntRect(56, 131, 4, 14));
		sf::Sprite bullet2(playerTexture, sf::IntRect(64, 131, 4, 12));
		bulletTexture1 = !bulletTexture1;
		bulletTimer = 0;
		for (int i = 0; i < projectiles.size(); i++)
		{
			if (bulletTexture1)
				projectiles[i]->setSprite(bullet1);
			else
				projectiles[i]->setSprite(bullet2);
		}
	}
}

void Game::spawnAsteroids()
{
	asteroidTimer += TimePerFrame.asSeconds();
	if (asteroidTimer >= 0.3f)
	{
		asteroidTimer = 0;
		sf::Texture& asteroidTexture = mTextures.get(Textures::Asteroid);
		sf::Sprite asteroid(asteroidTexture, sf::IntRect(0, 0, 64, 64));
		ExplodingThing* tempAsteroid(new ExplodingThing(ExplodingThing::Asteroid, mTextures, sf::Vector2f(0, asteroidSpeed)));
		float spawnArea = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 4; // Random float between 0-4.
		float spawnPosition = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // Random float 0-1.
		float spawnX, spawnY;
		if (spawnArea < 1)
		{ // North
			spawnX = spawnPosition * mWorldBounds.width;
			spawnY = -64;
		}
		else if (spawnArea < 2)
		{ // South
			spawnX = spawnPosition * mWorldBounds.width;
			spawnY = mWorldBounds.height + 64;
		}
		else if (spawnArea < 3)
		{ // West
			spawnX = -64;
			spawnY = spawnPosition * mWorldBounds.height;
		}
		else
		{
			spawnX = mWorldBounds.width + 64;
			spawnY = spawnPosition * mWorldBounds.height;
		}
		sf::Vector2f currentSpeed = calculateAsteroidDirection(sf::Vector2f(spawnX, spawnY));
		tempAsteroid->setPosition(spawnX, spawnY);
		tempAsteroid->setSpeed(currentSpeed);
		tempAsteroid->setSprite(asteroid);
		asteroids.push_back(tempAsteroid);
	}
}

void Game::moveAsteroids()
{
	/*
	 * a good thing is that asteroids do not "chase" the player
	 */
	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i]->move(asteroids[i]->getSpeed() * TimePerFrame.asSeconds());
		asteroids[i]->update(TimePerFrame);
		if (asteroids[i]->getPosition().x < -70 || asteroids[i]->getPosition().x > mWorldBounds.width + 70 ||
			asteroids[i]->getPosition().y < -70 || asteroids[i]->getPosition().y > mWorldBounds.height + 70)
		{
			asteroids.erase(asteroids.begin() + i);
		}
	}
}

sf::Vector2f Game::calculateAsteroidDirection(sf::Vector2f position)
{
	float asteroidBehaviour = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 3; // Random float between 0-3.

	if (asteroidBehaviour < 1) {
		/* follows the player */
		float distanceX = mPlayer->getPosition().x - position.x;
		float distanceY = mPlayer->getPosition().y - position.y;
		float total = abs(distanceX) + abs(distanceY);
		return sf::Vector2f(distanceX / total, distanceY / total) * asteroidSpeed;
	}
	else if (asteroidBehaviour >= 1 && asteroidBehaviour <= 2){
		float random = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		return sf::Vector2f(random, (1- random)) * asteroidSpeed;
	}
	else {
		// Goes to the center of the screen 
		float distanceX = 1280/2 -position.x;
		float distanceY = 900/2 - position.y;
		float total = abs(distanceX) + abs(distanceY);
		return sf::Vector2f(distanceX / total, distanceY / total) * asteroidSpeed;
	}

}

void Game::animateAsteroids()
{
	asteroidAnimationTimer += TimePerFrame.asSeconds();
	if (asteroidAnimationTimer >= 0.05f)
	{
		sf::Texture& asteroidTexture = mTextures.get(Textures::Asteroid);
		vector<sf::Sprite> sprites;
		for (int i = 0; i < 16; i++)
		{
			sprites.push_back(sf::Sprite(asteroidTexture, sf::IntRect(0 + i * 64, 0, 64, 64)));
		}
		asteroidAnimationTimer = 0;
		asteroidAnimationIndex++;
		if (asteroidAnimationIndex > 15)
			asteroidAnimationIndex = 0;
		for (int i = 0; i < asteroids.size(); i++)
		{
			asteroids[i]->setSprite(sprites[asteroidAnimationIndex]);
		}
	}
}

void Game::collisionBulletAsteroid()
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		float asteroidLeft = asteroids[i]->getPosition().x;
		float asteroidRight = asteroids[i]->getPosition().x + asteroids[i]->getSprite().getGlobalBounds().width;
		float asteroidTop = asteroids[i]->getPosition().y;
		float asteroidBottom = asteroids[i]->getPosition().y + asteroids[i]->getSprite().getGlobalBounds().height;
		for (int j = 0; j < projectiles.size(); j++)
		{
			float projectileLeft = projectiles[j]->getPosition().x;
			float projectileRight = projectiles[j]->getPosition().x + projectiles[j]->getSprite().getGlobalBounds().width;
			float projectileTop = projectiles[j]->getPosition().y;
			float projectileBottom = projectiles[j]->getPosition().y + projectiles[j]->getSprite().getGlobalBounds().height;
			if (asteroidRight > projectileLeft && asteroidLeft < projectileRight)
			{
				if (asteroidBottom > projectileTop && asteroidTop < projectileBottom)
				{
					addExplosion(asteroids[i]->getPosition());
					asteroids.erase(asteroids.begin() + i);
					projectiles.erase(projectiles.begin() + j);
				}
			}
		}
	}
}

void Game::addExplosion(sf::Vector2f position)
{
	sf::Texture& explosionTexture = mTextures.get(Textures::Spaceship);
	sf::Sprite explosion(explosionTexture, sf::IntRect(0, 0, 64, 64));
	ExplodingThing* tempExplosion(new ExplodingThing(ExplodingThing::Spaceship, mTextures, sf::Vector2f(0, 0)));
	tempExplosion->setPosition(position.x, position.y);
	tempExplosion->setSprite(explosion);
	explosions.push_back(tempExplosion);
}

void Game::animateExplosions()
{
	expAnimTimer += TimePerFrame.asSeconds();
	if (expAnimTimer >= 0.01f)
	{
		sf::Texture& explosionTexture = mTextures.get(Textures::Explosion);
		vector<sf::Sprite> sprites;
		for (int i = 0; i < 64; i++)
		{
			sprites.push_back(sf::Sprite(explosionTexture, sf::IntRect(0 + i * 64, 0, 64, 64)));
		}
		expAnimTimer = 0;
		for (int i = 0; i < explosions.size(); i++)
		{
			explosions[i]->setSprite(sprites[explosions[i]->getExpAnimIndex()]);
			explosions[i]->setExpAnimIndex(explosions[i]->getExpAnimIndex() + 1);
			if (explosions[i]->getExpAnimIndex() > 63)
			{
				explosions.erase(explosions.begin() + i);
			}
		}
	}
}

void Game::collisionPlayerAsteroid()
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		float asteroidLeft = asteroids[i]->getPosition().x;
		float asteroidRight = asteroids[i]->getPosition().x + asteroids[i]->getSprite().getGlobalBounds().width;
		float asteroidTop = asteroids[i]->getPosition().y;
		float asteroidBottom = asteroids[i]->getPosition().y + asteroids[i]->getSprite().getGlobalBounds().height;
		float playerLeft = mPlayer->getPosition().x;
		float playerRight = mPlayer->getPosition().x + mPlayer->getSprite().getGlobalBounds().width;
		float playerTop = mPlayer->getPosition().y;
		float playerBottom = mPlayer->getPosition().y + mPlayer->getSprite().getGlobalBounds().height;
		if (asteroidRight > playerLeft && asteroidLeft < playerRight)
		{
			if (asteroidBottom > playerTop && asteroidTop < playerBottom)
			{
				addExplosion(asteroids[i]->getPosition());
				asteroids.erase(asteroids.begin() + i);
				activeButton = "retry";
				gameState = 3;
				mPlayer->setPosition(-100, -100);
			}
		}
	}
}

void Game::pauseMenu()
{
	menuTimer += TimePerFrame.asSeconds();
	if (menuTimer >= 0.5f)
	{
		menuActive = true;
		menuTimer = 0;
	}
	if (menuActive)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			menuActive = false;
			if (activeButton == "resume")
				activeButton = "mainMenuPause";
			else
				activeButton = "resume";
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && activeButton == "resume")
		{
			menuActive = false;
			gameState = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && activeButton == "mainMenuPause")
		{
			menuTimer = 0;
			menuActive = false;
			gameState = 0;
			activeButton = "play";
		}
	}
}

void Game::gameOver()
{
	menuTimer += TimePerFrame.asSeconds();
	if (menuTimer >= 0.5f)
	{
		menuActive = true;
		menuTimer = 0;
	}
	if (menuActive)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			menuActive = false;
			if (activeButton == "retry")
				activeButton = "mainMenuGameOver";
			else
				activeButton = "retry";
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && activeButton == "retry")
		{
			menuActive = false;
			gameState = 1;
			initializeGame();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && activeButton == "mainMenuGameOver")
		{
			menuTimer = 0;
			menuActive = false;
			gameState = 0;
			activeButton = "play";
		}
	}
}
