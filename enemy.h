#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Enemy {
public:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    bool movingRight;
    sf::Texture spriteSheet;
    sf::Texture deathTexture;
    std::vector<sf::IntRect> frames;
    std::vector<sf::IntRect> deathFrames;
    int currentFrame;
    float animationTimer;
    float animationSpeed;
    int health;
    sf::RectangleShape healthBar;
    
    bool isDead;
    bool isDying;
	sf::Clock damageClock;
	float lastDamageTime;

    Enemy(const std::string& texturePath, const std::string& deathTexturePath, float x, float y, float speed)
        : movingRight(true), currentFrame(0), animationTimer(0), animationSpeed(0.1f), health(30), isDead(false), isDying(false), lastDamageTime(0) {

        if (!spriteSheet.loadFromFile(texturePath)) {
            std::cerr << "Текстура не найдена!" << std::endl;
        }

        if (!deathTexture.loadFromFile(deathTexturePath)) {
            std::cerr << "Текстура не найдена!" << std::endl;
        }

        sprite.setTexture(spriteSheet);
        sprite.setPosition(x, y);

        const int SPRITE_WIDTH = 109;
        const int SPRITE_HEIGHT = 81;
        const int FRAME_COUNT = 7;

        for (int i = 0; i < FRAME_COUNT; i++) {
            frames.emplace_back(i * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
        }

        for (int i = 0; i < 4; i++) {
            deathFrames.emplace_back(i * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
        }

        velocity.x = speed;
        
        healthBar.setSize(sf::Vector2f(100.0f, 10.0f));
        healthBar.setFillColor(sf::Color::Green);
        healthBar.setPosition(x, y - 20);
    }

   void updateAnimation(float deltaTime) {
    if (isDying) {
            animationTimer += deltaTime;
            if (animationTimer >= animationSpeed) {
                animationTimer = 0;
                currentFrame++;
                if (currentFrame >= deathFrames.size()) {
                    isDead = true;
                }
            }

            sf::IntRect frame = deathFrames[currentFrame];
            sprite.setTexture(deathTexture);
            sprite.setTextureRect(frame);
        } else {
            animationTimer += deltaTime;
            if (animationTimer >= animationSpeed) {
                animationTimer = 0;
                currentFrame = (currentFrame + 1) % frames.size();
            }

            sf::IntRect frame = frames[currentFrame];
            if (!movingRight) {
                frame.left += frame.width;
                frame.width = -frame.width;
            }
            sprite.setTexture(spriteSheet);
            sprite.setTextureRect(frame);
        }
}
    void update(float deltaTime) {
     if (isDying || isDead) {
            return;
        }

        sprite.move(velocity * deltaTime);

        if (sprite.getPosition().x + sprite.getGlobalBounds().width > 2200) {
            movingRight = false;
            velocity.x = -velocity.x;
        }
        if (sprite.getPosition().x < 0) {
            movingRight = true;
            velocity.x = -velocity.x;
        }

        updateAnimation(deltaTime);

        healthBar.setSize(sf::Vector2f(100.0f * (health / 100.0f), 10.0f));
        healthBar.setPosition(sprite.getPosition().x, sprite.getPosition().y - 20);
        if (health <= 0 && !isDying) {
            isDying = true;
        }
}

    void draw(sf::RenderWindow& window) {
        if (!isDead) {
            window.draw(sprite);
            window.draw(healthBar);
        }
    }
    void takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
    }
	
	bool canDamage(float damageDelay) {
        if (damageClock.getElapsedTime().asSeconds() - lastDamageTime >= damageDelay) {
            lastDamageTime = damageClock.getElapsedTime().asSeconds();
            return true;
        }
        return false;
    }
};

#endif
