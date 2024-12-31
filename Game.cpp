#include <SFML/Graphics.hpp>
#include <iostream>
#include <locale>
#include "Level.h"
#include "Move.h"
#include "Background.h"
#include "Enemy.h"
#include "Portal.h"
#include "menu.h"
#include <locale>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 1024;
const float GRAVITY = 0.5f;
const float JUMP_SPEED = -12.0f;
const float PLAYER_SPEED = 5.0f;
const float SPAWN_X = 150.0f;
const float SPAWN_Y = 500.0f;
const float FALL_LIMIT = 2000.0f;
const float ENEMY_DAMAGE_DELAY = 0.5f;

// Enum для состояний игры
enum class GameState {
    MainMenu,
    LevelSelect,
    InGame,
    Settings,
    About,
};


class Player {
public:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    bool onGround;
    Movement movement;
    sf::Clock clock;
    int health;
	int coins;
    sf::RectangleShape healthBar;
	

    Player(float x, float y) : onGround(false), health(100), coins(0) {
        if (!movement.spriteSheet.loadFromFile("sprites/walk.png")) {
            std::cerr << "Error: Failed to load player sprite!" << std::endl;
        }

        sprite.setTexture(movement.spriteSheet);
        sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
        sprite.setScale(1.0f, 1.0f);
        sprite.setPosition(x, y);

        healthBar.setSize(sf::Vector2f(100.0f, 10.0f));
        healthBar.setFillColor(sf::Color::Green);
		
    }

    void update(Level& level, float deltaTime) {
        if (!onGround) {
            velocity.y += GRAVITY;
        }

        sprite.move(velocity);
        movement.updateAnimation(sprite, deltaTime, velocity.x);

        onGround = false;
        for (auto& platform : level.platforms) {
            if (sprite.getGlobalBounds().intersects(platform.sprite.getGlobalBounds()) && !platform.isCoin) {
                if (velocity.y > 0) {
                    sprite.setPosition(sprite.getPosition().x, platform.sprite.getPosition().y - sprite.getGlobalBounds().height);
                    velocity.y = 0;
                    onGround = true;
                }
            }
        }

        if (sprite.getPosition().y > FALL_LIMIT) {
            respawn();
        }
		
        healthBar.setSize(sf::Vector2f(100.0f * (health / 100.0f), 10.0f));
        if (health <= 0) {
            respawn();
        }
    }
	
	void addCoin(){
		coins++;
	}

    void respawn() {
        sprite.setPosition(SPAWN_X, SPAWN_Y);
        velocity = sf::Vector2f(0, 0);
        health = 100;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }
};



int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Platform");
    window.setFramerateLimit(60);
    setlocale(LC_ALL, "Russian");

    GameState gameState = GameState::MainMenu;
    int selectedLevel = -1;
	bool levelSelected = false;
    Player player(SPAWN_X, SPAWN_Y);
    Level level;
    Enemy enemy("sprites/enemy.png", "sprites/enemy_death.png", 1800.0f, 420.0f, 50.0f);
    Portal portal(7800.0f, 437.0f, "sprites/portal_sprite.png", 6, 1, 0.1f, 119.5f, 79.0f);
    
    sf::RectangleShape leftBarrier(sf::Vector2f(10.0f, WINDOW_HEIGHT));
	leftBarrier.setPosition(130.0f, 0.0f);
	leftBarrier.setFillColor(sf::Color::Transparent);

	sf::RectangleShape rightBarrier(sf::Vector2f(10.0f, WINDOW_HEIGHT));
	rightBarrier.setPosition(8000.0f, 0.0f);
	rightBarrier.setFillColor(sf::Color::Transparent);

    sf::View view = window.getView();
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: Failed to load font!" << std::endl;
        return -1;
    }

    sf::Text coordinatesText;
    coordinatesText.setFont(font);
    coordinatesText.setCharacterSize(24);
    coordinatesText.setFillColor(sf::Color::White);
	
    sf::Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::Red);
	
    sf::Text coinText;
    coinText.setFont(font);
    coinText.setCharacterSize(24);
    coinText.setFillColor(sf::Color::Yellow);
	
	sf::Texture coinTexture;
	if (!coinTexture.loadFromFile("sprites/coin.png")) {
            std::cerr << "Error: Failed to load coin texture!" << std::endl;
    }
	sf::Sprite coinSprite;
	coinSprite.setTexture(coinTexture);
	coinSprite.setScale(0.07f,0.07f);

    Background background("sprites/background.png", 2000.0f, 1000.0f, 5, -600.0f, -100.0f);
	Background menuBackground("sprites/background.jpg", 2000.0f, 1000.0f, 5, 0, 0);

    sf::Clock clock;

	std::vector<Button> menuButtons;
	menuButtons.push_back(Button(sf::Vector2f(200, 50), sf::Vector2f(WINDOW_WIDTH / 2 - 100, 300), "Начать игру", font));
	menuButtons.push_back(Button(sf::Vector2f(200, 50), sf::Vector2f(WINDOW_WIDTH / 2 - 100, 380), "Об игре", font));
	menuButtons.push_back(Button(sf::Vector2f(200, 50), sf::Vector2f(WINDOW_WIDTH / 2 - 100, 460), "Настройки", font));
    menuButtons.push_back(Button(sf::Vector2f(200, 50), sf::Vector2f(WINDOW_WIDTH / 2 - 100, 540), "Выход", font));

    std::vector<Button> levelButtons;
    for(int i = 0; i < 3; i++){
		std::string levelText = "Уровень " + std::to_string(i+1);
        levelButtons.push_back(Button(sf::Vector2f(150, 50), sf::Vector2f(150 + i * 200, 300),  levelText , font));
    }
    

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
             if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                 sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if(gameState == GameState::MainMenu){
                    for (size_t i = 0; i < menuButtons.size(); ++i) {
                        if (menuButtons[i].isHovered) {
                            switch (i) {
                            case 0:
                                gameState = GameState::LevelSelect;
                                break;
                            case 1:
                                gameState = GameState::About;
                                break;
                            case 2:
                                gameState = GameState::Settings;
                                break;
                            case 3:
                                window.close();
                                break;
                            }
                        }
                    }
				} else if (gameState == GameState::LevelSelect){
					for (size_t i = 0; i < levelButtons.size(); ++i) {
						if(levelButtons[i].isHovered){
							selectedLevel = i;
                            levelSelected = true;
							for(auto& button: levelButtons){
                                button.isSelected = false;
                            }
                            levelButtons[i].isSelected = true;
						}
					}
				}
            }
        }
		

        float deltaTime = clock.restart().asSeconds();
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (gameState == GameState::MainMenu) {
                for(auto& button : menuButtons) {
                  button.update(mousePos);
                }
                drawMenu(window, font, menuButtons, menuBackground);
        }
        else if (gameState == GameState::LevelSelect){
				for(auto& button : levelButtons){
					button.update(mousePos);
				}
				drawMenu(window, font, levelButtons, menuBackground);
			if(levelSelected){
					gameState = GameState::InGame;
                }
		}
		else if(gameState == GameState::About){
			sf::Text aboutText;
			aboutText.setFont(font);
			aboutText.setString(sf::String("Эта игра создана, чтобы стать \n самой лучшей игрой на земле!"));
            aboutText.setCharacterSize(24);
			aboutText.setFillColor(sf::Color::White);
			aboutText.setPosition(WINDOW_WIDTH/2 - 200, 300);
			menuBackground.draw(window);
			window.draw(aboutText);
			sf::RectangleShape backRect(sf::Vector2f(100,50));
			backRect.setPosition(WINDOW_WIDTH/2 - 50, 600);
			backRect.setFillColor(sf::Color(100, 100, 100));
			sf::Text backText;
			backText.setFont(font);
			backText.setString(sf::String("Назад"));
			backText.setCharacterSize(24);
			backText.setFillColor(sf::Color::White);
			sf::FloatRect backTextRect = backText.getLocalBounds();
            backText.setOrigin(backTextRect.left + backTextRect.width / 2.0f, backTextRect.top + backTextRect.height / 2.0f);
            backText.setPosition(WINDOW_WIDTH/2, 625);
			if (backRect.getGlobalBounds().contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                gameState = GameState::MainMenu;
            }
			
			window.draw(backRect);
			window.draw(backText);
			window.display();
		} else if (gameState == GameState::Settings){
			sf::Text settingsText;
			settingsText.setFont(font);
			settingsText.setString(sf::String("В разработке"));
            settingsText.setCharacterSize(24);
			settingsText.setFillColor(sf::Color::White);
			settingsText.setPosition(WINDOW_WIDTH/2 - 100, 300);
			menuBackground.draw(window);
			window.draw(settingsText);
			sf::RectangleShape backRect(sf::Vector2f(100,50));
			backRect.setPosition(WINDOW_WIDTH/2 - 50, 600);
			backRect.setFillColor(sf::Color(100, 100, 100));
			sf::Text backText;
			backText.setFont(font);
			backText.setString(sf::String("Назад"));
			backText.setCharacterSize(24);
			backText.setFillColor(sf::Color::White);
			sf::FloatRect backTextRect = backText.getLocalBounds();
            backText.setOrigin(backTextRect.left + backTextRect.width / 2.0f, backTextRect.top + backTextRect.height / 2.0f);
            backText.setPosition(WINDOW_WIDTH/2, 625);
			if (backRect.getGlobalBounds().contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                gameState = GameState::MainMenu;
            }
			
			window.draw(backRect);
			window.draw(backText);
			window.display();
		}
        else if (gameState == GameState::InGame) {
		    
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                if (player.sprite.getGlobalBounds().intersects(leftBarrier.getGlobalBounds())) {
                    player.velocity.x = 0; 
                } else {
                    player.velocity.x = -PLAYER_SPEED;
                }
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                if (player.sprite.getGlobalBounds().intersects(rightBarrier.getGlobalBounds())) {
                    player.velocity.x = 0; 
                } else {
                    player.velocity.x = PLAYER_SPEED;
                }
            } else {
                player.velocity.x = 0;
            }
    
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player.onGround) {
                player.velocity.y = JUMP_SPEED;
                player.onGround = false;
            }

            player.update(level, deltaTime);
            background.update(player.velocity.x, deltaTime);
            enemy.update(deltaTime);
            portal.update(deltaTime);

            if (player.sprite.getGlobalBounds().intersects(enemy.sprite.getGlobalBounds())) {
                if (player.velocity.y > 0 && player.sprite.getPosition().y + player.sprite.getGlobalBounds().height <= enemy.sprite.getPosition().y + 10) {
                    enemy.takeDamage(10);
                    player.velocity.y = JUMP_SPEED;
                } else if(enemy.canDamage(ENEMY_DAMAGE_DELAY)){
                    player.takeDamage(10);
				}
            }
		
            for (auto& coin : level.coins) {
                if (!coin.isCollected && player.sprite.getGlobalBounds().intersects(coin.sprite.getGlobalBounds())){
                    player.addCoin();
                    coin.isCollected = true;
                }
            }

            view.setCenter(player.sprite.getPosition().x + player.sprite.getGlobalBounds().width / 2,
                        player.sprite.getPosition().y + player.sprite.getGlobalBounds().height / 2);
            window.setView(view);

            coordinatesText.setString("X: " + std::to_string(static_cast<int>(player.sprite.getPosition().x)) +
                                    " Y: " + std::to_string(static_cast<int>(player.sprite.getPosition().y)));
            coordinatesText.setPosition(player.sprite.getPosition().x, player.sprite.getPosition().y - 30);
		
            healthText.setString("Health: " + std::to_string(player.health));
            healthText.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + 20, view.getCenter().y - WINDOW_HEIGHT / 2 + 20);
		
            coinText.setString("Coins: " + std::to_string(player.coins));
            coinText.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + 20, view.getCenter().y - WINDOW_HEIGHT / 2 + 50);
		
            coinSprite.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + 110, view.getCenter().y - WINDOW_HEIGHT / 2 + 45);

		    player.healthBar.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + 20, view.getCenter().y - WINDOW_HEIGHT / 2 + 10);

            window.clear(sf::Color(64,65,244));
            background.draw(window);
            for (auto& platform : level.platforms) {
                window.draw(platform.sprite);
            }

            level.render(window);
		    portal.draw(window);
            window.draw(player.sprite);
		    window.draw(player.healthBar);
            enemy.draw(window);
            window.draw(coordinatesText);
		    window.draw(leftBarrier);
		    window.draw(rightBarrier);
		    window.draw(healthText);
		    window.draw(coinText);
		    window.draw(coinSprite);
            window.display();
        }
    }

    return 0;
}
