//
// Created by ayesdie on 02/10/20.
//

#ifndef CHARMS_SNAKE_HPP
#define CHARMS_SNAKE_HPP

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>

namespace ch {
    enum class direction {UP, RIGHT, DOWN, LEFT};

    class snake {
    private:
        std::vector<std::pair<int, int>> snake_body;
        std::pair<int, int> food_location;
        sf::Color snake_color;
        sf::Color food_color;

        size_t snake_moves;
        size_t score;

        bool isAlive;

        direction current_direction;
        direction new_direction;

        sf::RenderWindow* window;

        const sf::Vector2f pair_to_pos(const std::pair<int, int>& pair) {
            return sf::Vector2f(pair.first * 15, pair.second * 15);
        }

        bool is_colliding(std::pair<int, int> head) {
            if (head.first < 1 || head.first > 40)
                return true;

            if (head.second < 1 || head.second > 40)
                return true;

            return is_colliding_body(head);
        }

        bool is_colliding_body(std::pair<int, int> head) {
            for (const auto &item : snake_body) {
                if ((head.first == item.first) && (head.second == item.second))
                    return true;
            }

            return false;
        }

        std::pair<int, int> get_new_food_location() {
            std::pair<int, int> temp_location;
            do {
                temp_location = std::pair<int, int>((1 + (39 * (float) rand() / RAND_MAX)),(1 + (39 * (float) rand() / RAND_MAX)));
            } while (is_colliding(temp_location));
            return temp_location;
        }

    public:
        snake() {
            score = 3;

            current_direction = direction::RIGHT;
            new_direction = direction::DOWN;
            snake_moves = 1000;
            isAlive = true;

            food_location = get_new_food_location();

            int R = 100 + (155 * ((float) rand() / RAND_MAX));
            int G = 100 + (155 * ((float) rand() / RAND_MAX));
            int B = 100 + (155 * ((float) rand() / RAND_MAX));
            int A = 150;
            snake_color = sf::Color(R, G, B, A);
            food_color = sf::Color(R, G, B);

            snake_body.emplace_back(std::pair<int, int>(5,7));
            snake_body.emplace_back(std::pair<int, int>(5,6));
            snake_body.emplace_back(std::pair<int, int>(5,5));
        }

        direction get_current_direction() { return current_direction; }
        void set_new_direction(direction dir) { new_direction = dir; }

        void draw(sf::RenderWindow* window) {
            sf::RectangleShape tile(sf::Vector2f(15, 15));
            tile.setFillColor(snake_color);
            for (auto &item : snake_body) {
                tile.setPosition(pair_to_pos(item));
                window->draw(tile);
            }

            tile.setFillColor(food_color);
            tile.setPosition(pair_to_pos(food_location));
            window->draw(tile);
        }

        void move() {
            if (isAlive) {
                std::pair<int, int> new_head;

                switch (new_direction) {
                    case direction::UP:
                        new_head.first = snake_body[0].first;
                        new_head.second = snake_body[0].second - 1;
                        break;
                    case direction::RIGHT:
                        new_head.first = snake_body[0].first + 1;
                        new_head.second = snake_body[0].second;
                        break;
                    case direction::DOWN:
                        new_head.first = snake_body[0].first;
                        new_head.second = snake_body[0].second + 1;
                        break;
                    case direction::LEFT:
                        new_head.first = snake_body[0].first - 1;
                        new_head.second = snake_body[0].second;
                        break;
                }

                if (is_colliding(new_head)) {
                    isAlive = false;
                }

                if (new_head == food_location)
                {
                    score++;
                    snake_body.insert(snake_body.begin(), new_head);
                    food_location = get_new_food_location();
                }
                else
                {
                    snake_body.insert(snake_body.begin(), new_head);
                    snake_body.pop_back();
                }

                current_direction = new_direction;
            }
        }

        std::vector<float> intellisense() {
            std::vector<float> sense;
            auto head = snake_body[0];

            // V1
            sense.emplace_back(head.second);
            sense.emplace_back(head.first - food_location.first == 0 && head.second - food_location.second > 0 ? 1 : 0);
            sense.emplace_back(0);
            for (int i = head.second - 1; i > 0; i--) {
                if (is_colliding_body(std::pair<int, int>(head.first, i))) {
                    sense.pop_back();
                    sense.emplace_back(1);
                    break;
                }
            }

            // V2
            sense.emplace_back(std::min(std::sqrt(2 * std::pow(head.second, 2)), std::sqrt(2 * std::pow(41 - head.first, 2))));
            sense.emplace_back(head.first - food_location.first < 0 && head.first - food_location.first == - (head.second - food_location.second) ? 1 : 0);
            sense.emplace_back(0);
            for (int i = head.first + 1, j = head.second - 1; i < 41 && j > 0; i++, j--) {
                if (is_colliding_body(std::pair<int, int>(i, j))) {
                    sense.pop_back();
                    sense.emplace_back(1);
                    break;
                }
            }

            // V3
            sense.emplace_back(41 - head.first);
            sense.emplace_back(head.first - food_location.first < 0 && head.second - food_location.second == 0 ? 1 : 0);
            sense.emplace_back(0);
            for (int i = head.first + 1; i < 41; i ++) {
                if (is_colliding_body(std::pair<int, int>(i, head.second))) {
                    sense.pop_back();
                    sense.emplace_back(1);
                    break;
                }
            }

            // V4
            sense.emplace_back(std::min(std::sqrt(2 * std::pow(41 - head.first, 2)), std::sqrt(2 * std::pow(41 - head.second, 2))));
            sense.emplace_back(head.first - food_location.first < 0 && head.first - food_location.first == head.second - food_location.second ? 1 : 0);
            sense.emplace_back(0);
            for (int i = head.first + 1, j = head.second + 1; i < 41 && j < 41; i++, j++) {
                if (is_colliding_body(std::pair<int, int>(i, j))) {
                    sense.pop_back();
                    sense.emplace_back(1);
                    break;
                }
            }

            // V5
            sense.emplace_back(41 - head.second);
            sense.emplace_back(head.first - food_location.first == 0 && head.second - food_location.second < 0 ? 1 : 0);
            sense.emplace_back(0);
            for (int i = head.second + 1; i < 41; i++) {
                if (is_colliding_body(std::pair<int, int>(head.first, i))) {
                    sense.pop_back();
                    sense.emplace_back(1);
                    break;
                }
            }

            // V6
            sense.emplace_back(std::min(std::sqrt(2 * std::pow(41 - head.second, 2)), std::sqrt(2 * std::pow(head.first, 2))));
            sense.emplace_back(head.first - food_location.first > 0 && head.first - food_location.first == - (head.second - food_location.second) ? 1 : 0);
            sense.emplace_back(0);
            for (int i = head.first - 1, j = head.second + 1; i > 0 && j < 41; i--, j++) {
                if (is_colliding_body(std::pair<int, int>(i, j))) {
                    sense.pop_back();
                    sense.emplace_back(1);
                    break;
                }
            }

            // V7
            sense.emplace_back(head.first);
            sense.emplace_back(head.first - food_location.first > 1 && head.second - food_location.second == 0 ? 1 : 0);
            sense.emplace_back(0);
            for (int i = head.first - 1; i > 0; i --) {
                if (is_colliding_body(std::pair<int, int>(i, head.second))) {
                    sense.pop_back();
                    sense.emplace_back(1);
                    break;
                }
            }

            // V8
            sense.emplace_back(std::min(std::sqrt(2 * std::pow(head.first, 2)), std::sqrt(2 * std::pow(head.second, 2))));
            sense.emplace_back(head.first - food_location.first > 0 && head.first - food_location.first == head.second - food_location.second ? 1 : 0);
            sense.emplace_back(0);
            for (int i = head.first - 1, j = head.second - 1; i > 0 && j > 0; i--, j--) {
                if (is_colliding_body(std::pair<int, int>(i, j))) {
                    sense.pop_back();
                    sense.emplace_back(1);
                    break;
                }
            }

            sense.emplace_back(current_direction == direction::UP ? 1 : 0);
            sense.emplace_back(current_direction == direction::RIGHT ? 1 : 0);
            sense.emplace_back(current_direction == direction::DOWN ? 1 : 0);
            sense.emplace_back(current_direction == direction::LEFT ? 1 : 0);

            auto tail = snake_body[snake_body.size() - 1];
            auto tail_1 = snake_body[snake_body.size() - 2];

            sense.emplace_back(tail.second - tail_1.second > 0 ? 1 : 0);
            sense.emplace_back(tail.first - tail_1.first < 0 ? 1 : 0);
            sense.emplace_back(tail.second - tail_1.second < 0 ? 1 : 0);
            sense.emplace_back(tail.first - tail_1.first > 0 ? 1 : 0);

            return sense;
        }
    };
}

#endif //CHARMS_SNAKE_HPP
