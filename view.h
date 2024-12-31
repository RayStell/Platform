#ifndef VIEW_H
#define VIEW_H

#include <SFML/Graphics.hpp>

class View {
public:
    sf::RenderWindow& window;
    sf::RectangleShape& playerShape;

    View(sf::RenderWindow& win, sf::RectangleShape& player) 
        : window(win), playerShape(player) {}

    void update() {
        sf::View view = window.getView();
        //  амера будет следовать за центром игрока
        view.setCenter(playerShape.getPosition().x + playerShape.getSize().x / 2,
                       playerShape.getPosition().y + playerShape.getSize().y / 2);
        window.setView(view);
    }
};

#endif

