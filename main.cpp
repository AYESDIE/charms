#include <iostream>

#include <SFML/Graphics.hpp>
#include <string>
#include "src/population.hpp"

void draw_boundary(sf::RenderWindow* window) {
    sf::RectangleShape B;
    B.setFillColor(sf::Color(255, 255, 255));

    B = sf::RectangleShape(sf::Vector2f(15, 630));
    B.setPosition(0, 0);
    window->draw(B);

    B = sf::RectangleShape(sf::Vector2f(630, 15));
    B.setPosition(0, 0);
    window->draw(B);

    B = sf::RectangleShape(sf::Vector2f(15, 630));
    B.setPosition(615, 0);
    window->draw(B);

    B = sf::RectangleShape(sf::Vector2f(630, 15));
    B.setPosition(0, 615);
    window->draw(B);
}

void draw_intellisense(std::vector<double> intellisense, sf::RenderWindow* window) {
    sf::Text text;
    sf::Font font;

    font.loadFromFile("../arial.ttf");

    text.setFont(font);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setCharacterSize(10);

    for (int i = 0; i < intellisense.size(); i++) {
        text.setString(std::to_string(int(intellisense[i])));
        text.setPosition(50 * (i / 3), 700 + 20 * (i % 3));
        window->draw(text);
    }
}

int main() {
    auto window = new sf::RenderWindow(sf::VideoMode(630, 720), "snakes",sf::Style::Titlebar | sf::Style::Close);

    ch::population pop(100);

    sf::Clock clock;
    sf::Time time;

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window->close();
                    break;
            }
        }

        time = clock.getElapsedTime();
        if (time.asMilliseconds() >= 0.001)
        {
            if (pop.all_dead())
            {
                pop.natural_selection();
                pop.mutate(0.01);
            }
            else
            {
                window->clear(sf::Color::Black);
                draw_boundary(window);

                pop.draw(window);
                window->display();

                pop.update();

                clock.restart();
            }
        }
    }

    return 0;
}
