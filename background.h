#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <stdexcept>

class Background {
private:
    sf::Texture texture;
    std::vector<sf::Sprite> sprites;
    float scrollSpeed;

public:
    Background(const std::string& texturePath, float width, float height, int repeatCount, float startX = 0.0f, float startY = 0.0f)
        : scrollSpeed(0.0f) {
        if (!texture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load background texture!");
        }

        texture.setRepeated(true);

        for (int i = 0; i < repeatCount; ++i) {
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(width), static_cast<int>(texture.getSize().y)));
            sprite.setScale(1.0f, height / static_cast<float>(texture.getSize().y)); 
            sprite.setPosition(startX + i * width, startY);
            sprites.push_back(sprite);
        }
    }

     void update(float playerVelocityX, float deltaTime) {
        if (playerVelocityX != 0) {
            scrollSpeed = -playerVelocityX * 0.5f;
            for (auto& sprite : sprites) {
                sprite.move(scrollSpeed * deltaTime, 0);
            }
        }

        for (auto& sprite : sprites) {
            if (sprite.getPosition().x + sprite.getGlobalBounds().width < 0) {
                sprite.setPosition(sprite.getPosition().x + sprites.size() * sprite.getGlobalBounds().width, sprite.getPosition().y);
            }
            if (sprite.getPosition().x > sprites.size() * sprite.getGlobalBounds().width) {
                sprite.setPosition(sprite.getPosition().x - sprites.size() * sprite.getGlobalBounds().width, sprite.getPosition().y);
            }
        }
    }

    void draw(sf::RenderWindow& window) const {
        for (const auto& sprite : sprites) {
            window.draw(sprite);
        }
    }
};

#endif
