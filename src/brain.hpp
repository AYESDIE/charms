//
// Created by ayesdie on 03/10/20.
//

#ifndef CHARMS_BRAIN_HPP
#define CHARMS_BRAIN_HPP


#include <Eigen/Dense>
#include "core.hpp"

namespace ch {
    Eigen::VectorXd ReLu(const Eigen::MatrixXd& x){
        Eigen::MatrixXd temp = Eigen::MatrixXd::Identity(x.rows(), x.cols());
        for(int i = 0; i < x.rows(); ++i ) {
            for (int j = 0; j < x.cols(); ++j) {
                if (x(i, j) <= 0) {
                    temp(i, j) = (0.0);
                }
                else
                {
                    temp(i, j) = x(i, j);
                }
                std::cout << i << j << temp << std::endl;
            }
        }
        return temp;
    }

    class brain{
    private:
        std::vector<Eigen::MatrixXd> weights;

    public:
        brain() {
            weights.emplace_back(Eigen::MatrixXd::Random(32, 20));
            weights.emplace_back(Eigen::MatrixXd::Random(20, 12));
            weights.emplace_back(Eigen::MatrixXd::Random(12, 4));
        }

        direction calculate_next_move(std::vector<double>& intellisense) {
            Eigen::VectorXd input_layer_sense_in = Eigen::Map<Eigen::Matrix<double, 1, 32>>(intellisense.data());
            input_layer_sense_in.normalize();

            auto input_layer_sense_out = input_layer_sense_in.transpose() * weights[0];
            Eigen::MatrixXd hidden_layer_1_in(input_layer_sense_out.rows(), input_layer_sense_out.cols());
            for (int i = 0; i < input_layer_sense_out.rows(); ++i) {
                for (int j = 0; j < input_layer_sense_out.cols(); ++j) {
                    hidden_layer_1_in(i, j) = input_layer_sense_out(i, j) <= 0 ? 0 : input_layer_sense_out(i, j);
                }
            }

            auto hidden_layer_1_out = hidden_layer_1_in * weights[1];
            Eigen::MatrixXd hidden_layer_2_in(hidden_layer_1_out.rows(), hidden_layer_1_out.cols());
            for (int i = 0; i < hidden_layer_1_out.rows(); ++i) {
                for (int j = 0; j < hidden_layer_1_out.cols(); ++j) {
                    hidden_layer_2_in(i, j) = hidden_layer_1_out(i, j) <= 0 ? 0 : hidden_layer_1_out(i, j);
                }
            }

            auto hidden_layer_2_out = hidden_layer_2_in * weights[2];

            Eigen::MatrixXd::Index* max_index;
            hidden_layer_2_out.row(0).maxCoeff(max_index);

            switch (*max_index) {
                case 0:
                    return direction::UP;
                case 1:
                    return direction::RIGHT;
                case 2:
                    return direction::DOWN;
                case 3:
                    return direction::LEFT;
            }
        }


    };
}

#endif //CHARMS_BRAIN_HPP