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
        this->totalFrames = totalFrames; // ��� ������������ ���������� ������ � �������
        this->currentFrame = 0;
        this->frameWidth = frameWidth;
        this->frameHeight = frameHeight;

        // ������������� ��������� ��������� �������
        sprite.setPosition(x, y); // ������� �������
        // ������������� ������� �����
        textureRect = sf::IntRect(frameWidth * currentFrame, 0, frameWidth, frameHeight);
        sprite.setTextureRect(textureRect);
    }

    void update(float deltaTime) {
        // �� ������������ ���������� ������ �� 6 (������ 6 ������)
        int maxFrames = 6;
        
        if (animationClock.getElapsedTime().asSeconds() >= frameTime) {
            currentFrame = (currentFrame + 1) % maxFrames; // ���������� ������ ������ 6 ������

            // � ����� ������ ������ ����� ���� ������ � 1 ������, ������� ������� ��� ����� 0
            int currentColumn = currentFrame % (texture.getSize().x / frameWidth); // ������� (�� �����������)

            // ���������� ������� �������� ��� �������� �����
            textureRect = sf::IntRect(frameWidth * currentColumn, 0, frameWidth, frameHeight);
            sprite.setTextureRect(textureRect);
            animationClock.restart(); // ����� ������� ��� ���������� �����
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite); // ������������ ������, ��� ��������� ��� �������
    }
};

#endif

