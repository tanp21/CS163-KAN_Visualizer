#ifndef KANNET_H
#define KANNET_H

#include <bits/stdc++.h>
#include "utils.h"
#include "spline.h"
#include "kanlayer.h"

namespace KANN {

    class KAN {
        private:

        std::vector<KANLayer> layers;

        std::vector<float> forward(std::vector<float> x);

        void back_propagation(const std::vector<float> &y);

        public:

        KAN(const std::vector<int> &layer_in4, int spline_order = 3, int grid_size = grid.size());

        void train(const std::vector<std::pair<std::vector<float>, std::vector<float>>> &dataset, int epoch_size, \
        int num_epoch = 10000, float lr = .001);

        std::vector<float> predict(const std::vector<float> &x);

        void updateAll(float lr, int epoch_size);
    };
};

#endif