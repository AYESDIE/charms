//
// Created by ayesdie on 03/10/20.
//

#ifndef CHARMS_CORE_HPP
#define CHARMS_CORE_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <random>

namespace ch {
    enum class direction {UP, RIGHT, DOWN, LEFT};

    template <typename T>
    T random_please(T lower_bound, T upper_bound) {
        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_real_distribution<> dist(lower_bound,upper_bound);
        return dist(e2);
    }
}

#endif //CHARMS_CORE_HPP
