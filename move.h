#ifndef MOVE_H
#define MOVE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Movement {
public:
    sf::Texture spriteSheet;
    std::vector<sf::IntRect> frames; // Общий массив для всех направлений
    int currentFrame;
    float animationTimer;
    float animationSpeed;
    bool facingRight;

    Movement() : currentFrame(0), animationTimer(0), animationSpeed(0.1f), facingRight(true) {
        if (!spriteSheet.loadFromFile("sprites/walk.png")) {
            throw std::runtime_error("Error loading sprite sheet!");
        }

        const int SPRITE_WIDTH = 109;  // Ширина одного кадра
        const int SPRITE_HEIGHT = 81; // Высота одного кадра
        const int FRAME_COUNT = 8;     // Количество кадров

        for (int i = 0; i < FRAME_COUNT; i++) {
            frames.emplace_back(i * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
        }
    }

    void updateAnimation(sf::Sprite& sprite, float deltaTime, float velocityX) {
        animationTimer += deltaTime;

        if (velocityX > 0) {
            facingRight = true;
        } else if (velocityX < 0) {
            facingRight = false;
        }

        if (std::abs(velocityX) > 0.1f) { // Обновляем анимацию только при движении
            if (animationTimer >= animationSpeed) {
                animationTimer = 0;
                currentFrame = (currentFrame + 1) % frames.size();
            }
        } else {
            currentFrame = 0; // Сброс анимации на первый кадр, если персонаж остановился
        }

        // Устанавливаем текущий кадр
        sf::IntRect frame = frames[currentFrame];
        if (!facingRight) {
            frame.left += frame.width; // Смещаем левую границу
            frame.width = -frame.width; // Инверсия ширины
        }
        sprite.setTextureRect(frame);
    }
};

#endif

