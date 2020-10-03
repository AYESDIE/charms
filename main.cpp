#include <iostream>

#include <SFML/Graphics.hpp>
#include <string>
#include "src/snake.hpp"

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

void draw_intellisense(std::vector<float> intellisense, sf::RenderWindow* window) {
    sf::Text text;
    sf::Font font;

    font.loadFromFile("../arial.ttf");

    text.setFont(font);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setCharacterSize(10);

    for (int i = 0; i < intellisense.size(); i++) {
        text.setString(std::to_string(int(intellisense[i])));
        text.setPosition(50 * (i / 3), 670 + 20 * (i % 3));
        window->draw(text);
    }
}

int main() {
    srand(time(0));
    auto window = new sf::RenderWindow(sf::VideoMode(630, 720), "snakes",sf::Style::Titlebar | sf::Style::Close);

    ch::snake baby;

    sf::Clock clock;
    sf::Time time;


    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::W:
                            if (baby.get_current_direction() != ch::direction::DOWN)
                                baby.set_new_direction(ch::direction::UP);
                            break;
                        case sf::Keyboard::D:
                            if (baby.get_current_direction() != ch::direction::LEFT)
                                baby.set_new_direction(ch::direction::RIGHT);
                            break;
                        case sf::Keyboard::S:
                            if (baby.get_current_direction() != ch::direction::UP)
                                baby.set_new_direction(ch::direction::DOWN);
                            break;
                        case sf::Keyboard::A:
                            if (baby.get_current_direction() != ch::direction::RIGHT)
                                baby.set_new_direction(ch::direction::LEFT);
                            break;
                    }
                    break;
                case sf::Event::Closed:
                    window->close();
                    break;
            }
        }

        time = clock.getElapsedTime();
        if (time.asMilliseconds() >= 1000)
        {
            window->clear(sf::Color::Black);
            draw_boundary(window);
            baby.draw(window);
            draw_intellisense(baby.intellisense(), window);
            window->display();

            baby.move();
            clock.restart();
        }
    }

    return 0;
}
