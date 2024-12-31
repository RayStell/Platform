#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

struct Platform {
    sf::Sprite sprite;
    bool isCoin;
    Platform(float x, float y, float width, float height, const sf::Texture& texture, bool isCoin = false) {
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(width), static_cast<int>(height)));
        sprite.setPosition(x, y);
		this->isCoin = isCoin;
    }
};

struct Coin {
  sf::Sprite sprite;
  bool isCollected;

  Coin(float x, float y, const sf::Texture& texture) : isCollected(false) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.1f, 0.1f); // Подгоняем размер монеты
  }
};

class Level {
public:
    std::vector<Platform> platforms;
	std::vector<Coin> coins;
    sf::Texture groundTexture;
    sf::Texture platformTexture;
	sf::Texture coinTexture;

    Level() {
        if (!groundTexture.loadFromFile("sprites/ground.png")) {
            std::cerr << "Error: Failed to load ground texture!" << std::endl;
        }
        groundTexture.setRepeated(true);

        if (!platformTexture.loadFromFile("sprites/ground.png")) {
            std::cerr << "Error: Failed to load platform texture!" << std::endl;
        }
		
		if (!coinTexture.loadFromFile("sprites/coin.png")) {
            std::cerr << "Error: Failed to load coin texture!" << std::endl;
        }
		coinTexture.setRepeated(true);

        loadLevel();
    }

    void loadLevel() {
        platforms.push_back(Platform(-200, 500, 9000, 290, groundTexture));
        platforms.push_back(Platform(400, 350, 100, 20, platformTexture));
        platforms.push_back(Platform(600, 300, 100, 20, platformTexture));
        platforms.push_back(Platform(800, 250, 100, 20, platformTexture));
		
        coins.push_back(Coin(700, 400, coinTexture));
        coins.push_back(Coin(1000, 400, coinTexture));
    }

    void render(sf::RenderWindow& window) {
        for (auto& platform : platforms) {
            window.draw(platform.sprite);
        }
        for (const auto& coin : coins) {
          if (!coin.isCollected)
            window.draw(coin.sprite);
        }
    }
};

#endif
