#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ZombieArena.h"
#include "role.h"
#include "TextureHolder.h"
#include "bullet2.h"
#include "Pickup.h"
#include"map.h"
#include"item_manager.h"

using namespace sf;

int main()
{
	// Here is the instabce of TextureHolder
	TextureHolder holder;

	map kkp(10,10);
	std::list<Item*> itemList;

	// The game will always be in one of four states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	// Start with the GAME_OVER state
	State state = State::GAME_OVER;


	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"Zombie Arena", Style::Fullscreen);

	// Create a an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Here is our clock for timing everything
	Clock clock;
	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	// Create an instance of the Player class
	Player player(Vector2f(0,0));

	// The boundaries of the arena
	IntRect arena;

	// Create the background
	VertexArray background;
	// Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture(
		"graphics/background_sheet.png");

	// Prepare for a horde of zombies
	int numZombies;
	int numZombiesAlive;
	int numTraps;
	int numTrapsAlive;
	std::vector<Zombie> zombies;
	std::vector<Trap> traps;

	// 100 bullets should do
	std::list<Bullet*> bullets;
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	// When was the fire button last pressed?
	Time lastPressed;

	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Create a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// About the game
	int score = 0;
	int hiScore = 0;

	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	// Load the font
	Font font;
	font.loadFromFile("KOMIKAP_.ttf");

	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 150);
	gameOverText.setString("Press Enter to play\nW A S D for move\n left click to shoot\nEnjoy your fight");

	// Levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpText.setString(levelUpStream.str());

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	// Load the high score from a text file/
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 800);
	//zombiesRemainingText.setString("Zombies: 100");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1170, 980);
	//waveNumberText.setString("Wave: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);
		
	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;
	// What time was the last update
	Time timeSinceLastUpdate;
	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	// Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	sf::Sound splat;
	splat.setBuffer(splatBuffer);

	// Prepare the shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	// Prepare the reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	// Prepare the failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	// Prepare the powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	// Prepare the pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

	// The main game loop
	while (window.isOpen())
	{
		/*
		************
		Handle input
		************
		*/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				// Pause a game while playing
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
					wave = 0;
					score = 0;

					// Prepare the gun and ammo for next game
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					// Reset the player's stats
					player.resetPlayerStats();
				}

				if (state == State::PLAYING)
				{
					// Reloading
					
					
				}

			}
		}// End event polling


		 // Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle controls while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}

			// Fire a bullet
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{

				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
					> 200 / fireRate&& player.shoot(
						mouseWorldPosition.x, mouseWorldPosition.y, bullets))
				{

					// Pass the centre of the player and the centre of the crosshair
					// to the shoot function
					lastPressed = gameTimeTotal;
					shoot.play();
					//bulletsInClip--;
				}

			}// End fire a bullet

		}// End WASD while playing

		// Handle the levelling up state
		if (state == State::LEVELING_UP)
		{
			// Handle the player levelling up
			
				state = State::PLAYING;
			
				// Increase clip size
			

			if (state == State::PLAYING)
			{
				// Increase the wave number
				//wave++;

				// Prepare thelevel
				// We will modify the next two lines later'

				kkp.resize(170,100);
				kkp.setRandomRoom(1000);
				while (kkp.rooms.size() <= 5)
				{
					kkp.setRandomRoom(500);
				}
				kkp.beforeCreateMaze();
				kkp.makeRoute();
				kkp.degenerate();
				kkp.setbrick();


					

				arena.width = kkp.rooms[0]->Width*kkp.space[0][0]->Width;
				arena.height = kkp.rooms[0]->Height * kkp.space[0][0]->Height;
				arena.left = kkp.space[kkp.rooms[0]->Y][kkp.rooms[0]->X]->X;
				arena.top = kkp.space[kkp.rooms[0]->Y][kkp.rooms[0]->X]->Y;

				// Pass the vertex array by reference 
				// to the createBackground function
				int tileSize = createBackground(background, kkp);

				// Spawn the player in the middle of the arena
				player.spawn(kkp, resolution, tileSize);

				// Configure the pick-ups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				// Create a horde of zombies
				numZombies = 15 * wave;
				Vector2i zombiesAndTraps;
				

				// Delete the previously allocated memory (if it exists)
				std::vector<Zombie>().swap(zombies);
				std::vector<Trap>().swap(traps);

				zombiesAndTraps = createHorde(numZombies, kkp,player.roomnum,zombies,traps);
				numZombies = zombiesAndTraps.x;
				numTraps = zombiesAndTraps.y;

				numZombiesAlive = numZombies;
				numTrapsAlive = traps.size();

				// Play the powerup sound
				powerup.play();

				// Reset the clock so there isn't a frame jump
				clock.restart();
			}
		}// End levelling up

		 /*
		 ****************
		 UPDATE THE FRAME
		 ****************
		 */
		if (state == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();
			// Update the total game time
			gameTimeTotal += dt;
			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);

			// Set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			// Update the player
			player.update(dtAsSeconds,kkp);

			// Make a note of the players new position
			Vector2f playerPosition(player.getPosition());

			// Make the view centre around the player				
			mainView.setCenter(player.getPosition());

			


			// Loop through each Zombie and update them
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
			for (int i = 0; i < traps.size(); i++)
			{
				if (traps[i].isAlive())
				{
					traps[i].update(gameTimeTotal, playerPosition);
				}
			}
			
			// Update any bullets that are in-flight

			for (auto& bullet : bullets)
			{
				bullet->update(dtAsSeconds, kkp);
			}


			// Update the pickups
			//healthPickup.update(dtAsSeconds);
			//ammoPickup.update(dtAsSeconds);

			// Collision detection
			// Have any zombies been shot?
			for (std::list<Bullet*>::iterator bullet=bullets.begin();bullet!=bullets.end();bullet++)
			{
				if (! (*(bullet))->isDead())
				{
					for (int j = 0; j < zombies.size(); j++)
					{
						if (
							zombies[j].isAlive())
						{
							if ((*bullet)->getBound().intersects
							(zombies[j].getPosition()))
							{
								// Stop the bullet
								(*bullet)->stop();

								// Register the hit and see if it was a kill
								if (zombies[j].hit((*bullet)->getAttack(),gameTimeTotal,(*bullet)->getDic())) {
									// Not just a hit but a kill too
									score += 10;
									if (score >= hiScore)
									{
										hiScore = score;
									}
									Item* tmp;
									tmp=ItemManager::getItem(Vector2f(zombies[j].getPosition().left + zombies[j].getPosition().width / 2, zombies[j].getPosition().top + zombies[j].getPosition().height / 2));
									if (tmp != nullptr)
									{
										itemList.emplace_back(tmp);
									}
									numZombiesAlive--;

									// When all the zombies are dead (again)
									if (numZombiesAlive == 0 && numTrapsAlive == 0) {
										state = State::LEVELING_UP;
									}
								}

								// Make a splat sound
								splat.play();

							}
						}
					}
					for (int j = 0; j < traps.size(); j++)
					{
						if (
							traps[j].isAlive())
						{
							if ((*bullet)->getBound().intersects
							(traps[j].getPosition()))
							{
								// Stop the bullet
								(*bullet)->stop();

								// Register the hit and see if it was a kill
								if (traps[j].hit((*bullet)->getAttack(), gameTimeTotal, (*bullet)->getDic())) {
									// Not just a hit but a kill too
									score += 10;
									if (score >= hiScore)
									{
										hiScore = score;
									}
									Item* tmp;
									tmp = ItemManager::getItem(Vector2f(traps[j].getPosition().left + traps[j].getPosition().width / 2, traps[j].getPosition().top + traps[j].getPosition().height / 2));
									if (tmp != nullptr)
									{
										itemList.emplace_back(tmp);
									}
									numTrapsAlive--;

									// When all the zombies are dead (again)
									if (numZombiesAlive == 0 && numTrapsAlive == 0) {
										state = State::LEVELING_UP;
									}
								}

								// Make a splat sound
								splat.play();

							}
						}
					}
				}
				else
				{
					delete(*bullet);
					bullets.erase(bullet);
					if (bullets.size() > 0)
						bullet = bullets.begin();
					else
						break;
				}
				
			}// End zombie being shot

			// Have any zombies touched the player			
			for (int i = 0; i < zombies.size(); i++)
			{
				if (player.getBound().intersects
					(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					bool kk = false;
					kk=zombies[i].attackPlayer(gameTimeTotal, kkp, Vector2f(player.getBound().top + player.getBound().height / 2
						, player.getBound().left + player.getBound().width / 2));
					if (kk&&player.hit(gameTimeTotal,Vector2f(zombies[i].getPosition().top+ zombies[i].getPosition().height/2
						,zombies[i].getPosition().left+ zombies[i].getPosition().width/2), zombies[i].attk))
					{
						// More here later

						hit.play();
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
						
					}
				}
			}
			for (int i = 0; i < traps.size(); i++)
			{
				if (player.getBound().intersects
				(traps[i].getPosition()) && traps[i].isAlive())
				{
					int damage = traps[i].attack(gameTimeTotal, kkp, Vector2f(player.getBound().top + player.getBound().height / 2
						, player.getBound().left + player.getBound().width / 2));
					if (damage>0&&player.getDamage(damage))
					{
						// More here later

						hit.play();
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();

					}
				}
			}// End player touched

			for (auto& item : itemList)
			{
				if (player.getBound().intersects(item->getPosition()))
				{
					item->gotIt(&player);
				}
			}

			int iterp = 0;
			for (std::list<Item*>::iterator iter = itemList.begin(); iter != itemList.end(); )
			{
				(*iter)->update(dtAsSeconds);
				if ((*iter)->isDead())
				{
					delete(*iter);
					itemList.erase(iter);
					iter = itemList.begin();
					for (int i = 0; i < iterp; i++)
					{
						iter++;
					}
				}
				else
				{
					iter++;
					iterp++;
				}
			}

			// Has the player touched health pickup
			if (player.getBound().intersects
				(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealth(healthPickup.gotIt());
				// Play a sound
				pickup.play();
				
			}

			// Has the player touched ammo pickup
			if (player.getBound().intersects
				(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
				// Play a sound
				reload.play();
				
			}

			// size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			// Increment the amount of time since the last HUD update
			timeSinceLastUpdate += dt;
			// Increment the number of frames since the last HUD calculation
			framesSinceLastHUDUpdate++;
			// Calculate FPS every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{

				// Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				// Update the ammo text
				ssAmmo << player.getSp() << "/" << player.getMaxSp();
				ammoText.setString(ssAmmo.str());

				// Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				// Update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				// Update the wave
				ssWave << "nextCost:" << player.getSpCost();
				waveNumberText.setString(ssWave.str());

				// Update the high score text
				ssZombiesAlive << "Monster\nand\ntrap:" << numZombiesAlive+numTrapsAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;
				timeSinceLastUpdate = Time::Zero;
			}// End HUD update

		}// End updating the scene

		 /*
		 **************
		 Draw the scene
		 **************
		 */

		if (state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);
			//window.draw(spriteGameOver);
			// Draw the background
			window.draw(background, &textureBackground);

			// Draw the zombies
			for (int i = 0; i < traps.size(); i++)
			{
				window.draw(traps[i].getSprite__(1));
			}
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}
			

			for (auto bullet:bullets)
			{
				if (!bullet->isDead())
				{
					window.draw(bullet->getShape());
				}
			}
			for (auto pick : itemList)
			{
				window.draw(pick->getSprite());
			}
			// Draw the player
			window.draw(player.getSprite());

			// Draw the pickups is currently spawned
			
			//Draw the crosshair
			window.draw(spriteCrosshair);

			// Switch to the HUD view
			window.setView(hudView);

			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		window.display();

	}// End game loop

	return 0;
}