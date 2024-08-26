#ifndef KANNET_CPP
#define KANNET_CPP

#include "KAN.h"

namespace KANN {
    std::vector<float> KAN::forward(std::vector<float> x) {
        for (int i = 0; i < layers.size(); i++) {
            x = layers[i].forward_from(x);
        }
        return x;
    }

    void KAN::back_propagation(const std::vector<float> &y) {
        std::vector<float> grad = layers.back().calc_base_grad(y);
        for(int i = layers.size() - 1; ~i; i--) {
            grad = layers[i].backward(grad);
        }
    }

    KAN::KAN(const std::vector<int> &layer_in4, int spline_order, int grid_size) {
        layers.clear();
        for (int i = 1; i < layer_in4.size(); i++) {
            layers.push_back(KANLayer(layer_in4[i-1], layer_in4[i], spline_order));
        }
    }


    void KAN::train(const std::vector<std::pair<std::vector<float>, std::vector<float>>> &dataset, int epoch_size, \
    int num_epoch, float lr) {
        for (int i = 0; i < num_epoch; i++) {
            float loss = 0;
            for(auto &[x, y] : dataset) {
                forward(x);
                loss += layers.back().loss_func(y);
                back_propagation(y);
            }
            updateAll(lr, epoch_size);
            loss /= epoch_size;
            std::cerr << "LOSS = " << loss << "\n";
        }
    }

    std::vector<float> KAN::predict(const std::vector<float> &x) {
        return forward(x);
    }

    void KAN::updateAll(float lr, int epoch_size) {
        for (int i = (int)layers.size()-1; ~i; i--) {
            layers[i].updateAll(lr, epoch_size);
        }
    }
};

#endif