//
// Created by ayesdie on 03/10/20.
//

#ifndef CHARMS_POPULATION_HPP
#define CHARMS_POPULATION_HPP
#include "snake.hpp"
#include "core.hpp"

namespace ch {
    class population {
    private:
        size_t current_generation;
        size_t current_baby;
        size_t baby_per_generation;

        size_t best_score;
        double best_fitness_score;
        double fitness_score_sum;

        std::vector<snake> snakes;

        bool allDead;

    public:
        population(size_t babies) {
            current_generation = 1;
            baby_per_generation = babies;
            current_baby = 0;
            best_score = 0;
            best_fitness_score = 0;
            allDead = false;

            snakes = std::vector<snake>(baby_per_generation);
        }

        bool all_dead() { return allDead; }

        void draw(sf::RenderWindow* window) {
            snakes[current_baby].draw(window);

            sf::Text text;
            sf::Font font;

            font.loadFromFile("../arial.ttf");
            text.setFont(font);
            text.setFillColor(sf::Color(255, 255, 255));
            text.setCharacterSize(10);

            text.setString("Generation: " + std::to_string(current_generation));
            text.setPosition(10, 635);
            window->draw(text);

            text.setString("Baby: " + std::to_string(current_baby + 1) + " / " + std::to_string(baby_per_generation));
            text.setPosition(10, 645);
            window->draw(text);

            text.setString("Score: " + std::to_string(snakes[current_baby].get_score()));
            text.setPosition(10, 655);
            window->draw(text);

            text.setString("Moves Left: " + std::to_string(snakes[current_baby].moves_left()));
            text.setPosition(10, 665);
            window->draw(text);

            text.setString("Best Score: " + std::to_string(best_score));
            text.setPosition(100, 635);
            window->draw(text);

            text.setString("Best Fitness Score: " + std::to_string(best_fitness_score));
            text.setPosition(100, 645);
            window->draw(text);
        }

        void update() {
            snakes[current_baby].move();

            if (!snakes[current_baby].is_alive())
            {
                snakes[current_baby].calculate_fitness_score();
                if (snakes[current_baby].get_score() > best_score) {
                    best_score = snakes[current_baby].get_score();
                }

                if (snakes[current_baby].get_fitness_score() > best_fitness_score) {
                    best_fitness_score = snakes[current_baby].get_fitness_score();

                    snakes[current_baby].save_snake();
                }

                current_baby++;
            }

            if (current_baby == baby_per_generation)
            {
                allDead = true;
            }
        }

        size_t get_best_snake_index() {
            size_t i = 0;
            double f = 0;
            for (int j = 0; j < snakes.size(); j++) {
                if (snakes[j].get_fitness_score() > f) {
                    f = snakes[j].get_fitness_score();
                    i = j;
                }
            }
            return i;
        }

        snake get_fit_parent_snake() {
            auto rng = ch::random_uniform<double>(0, fitness_score_sum);
            double running_sum = 0;
            for (auto &snake : snakes) {
                running_sum += snake.get_fitness_score();
                if (running_sum > rng) {
                    return snake;
                }
            }

            return snakes[get_best_snake_index()];
        }

        void calculate_fitness_score() {
            fitness_score_sum = 0;
            for (auto &snake : snakes) {
                fitness_score_sum += snake.get_fitness_score();
            }
        }

        void natural_selection() {
            calculate_fitness_score();

            std::vector<snake> new_snakes;
            new_snakes.clear();

            new_snakes.push_back(snakes[get_best_snake_index()] * snakes[get_best_snake_index()]);
            for (int i = 1; i < baby_per_generation; ++i) {
                new_snakes.push_back(get_fit_parent_snake() * get_fit_parent_snake());
            }

            current_generation++;
            current_baby = 0;
            allDead = false;
            snakes = new_snakes;
        }

        void mutate(double mutation_rate) {
            for (int i = 1; i < snakes.size(); ++i) {
                snakes[i].mutate(mutation_rate);
            }
        }
    };
}

#endif //CHARMS_POPULATION_HPP
