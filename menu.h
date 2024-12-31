#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Background.h"

// Структура для кнопки
struct Button {
    sf::RectangleShape rect;
    sf::Text text;
    bool isHovered;
    bool isSelected;
    sf::Font font;

    Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::String& buttonText, sf::Font& font)
        : isHovered(false), isSelected(false) {
        rect.setSize(size);
        rect.setPosition(position);
        rect.setFillColor(sf::Color(100, 100, 100)); // Светло-серый цвет
        this->font = font;
        text.setFont(font);
        text.setString(buttonText);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
		
        // Выравнивание текста по центру кнопки
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
    }

    void update(const sf::Vector2f& mousePos) {
        isHovered = rect.getGlobalBounds().contains(mousePos);

        if(isHovered){
             rect.setFillColor(sf::Color(150,150,150));
        } else {
             rect.setFillColor(sf::Color(100,100,100));
        }
		
		 if(isSelected){
            rect.setFillColor(sf::Color(150, 250, 150));
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(rect);
        window.draw(text);
    }
};

// Функция для отрисовки меню
void drawMenu(sf::RenderWindow& window, sf::Font& font, const std::vector<Button>& buttons, const Background& background) {
    window.clear(sf::Color(64,65,244));
    background.draw(window);
    for(const auto& button : buttons){
        button.draw(window);
    }
    window.display();
}


#endif
