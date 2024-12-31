#ifndef PORTAL_H
#define PORTAL_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Portal {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect textureRect;
    float frameTime;
    int totalFrames;
    int currentFrame;
    sf::Clock animationClock;
    int frameWidth;
    int frameHeight;

    Portal(float x, float y, const std::string& texturePath, int totalFrames, int rows, float frameTime, int frameWidth, int frameHeight) {
        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "Error: Failed to load portal texture!" << std::endl;
        }

        sprite.setTexture(texture);
        this->frameTime = frameTime;
        this->totalFrames = totalFrames; // Ёто максимальное количество кадров в спрайте
        this->currentFrame = 0;
        this->frameWidth = frameWidth;
        this->frameHeight = frameHeight;

        // ”станавливаем начальное положение спрайта
        sprite.setPosition(x, y); // ѕозици€ портала
        // »нициализаци€ первого кадра
        textureRect = sf::IntRect(frameWidth * currentFrame, 0, frameWidth, frameHeight);
        sprite.setTextureRect(textureRect);
    }

    void update(float deltaTime) {
        // ћы ограничиваем количество кадров до 6 (первые 6 кадров)
        int maxFrames = 6;
        
        if (animationClock.getElapsedTime().asSeconds() >= frameTime) {
            currentFrame = (currentFrame + 1) % maxFrames; // »спользуем только первые 6 кадров

            // ¬ нашем случае портал может быть только в 1 строке, поэтому текущий р€д равен 0
            int currentColumn = currentFrame % (texture.getSize().x / frameWidth); //  олонка (по горизонтали)

            // ќбновление области текстуры дл€ текущего кадра
            textureRect = sf::IntRect(frameWidth * currentColumn, 0, frameWidth, frameHeight);
            sprite.setTextureRect(textureRect);
            animationClock.restart(); // —брос времени дл€ следующего кадра
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite); // ќтрисовываем портал, без изменени€ его позиции
    }
};

#endif

