#ifndef KANNET_H
#define KANNET_H

#include <bits/stdc++.h>
#include "utils.h"
#include "spline.h"
#include "kanlayer.h"
#include "visualizer.h"

namespace KANN {

    class KAN {
        private:

        std::vector<KANLayer> layers;

        std::vector<float> forward(std::vector<float> x);

        void back_propagation(const std::vector<float> &y);

        public:
        const std::vector<KANLayer>& getLayers() const;

        KAN(const std::vector<int> &layer_in4, int spline_order = 3, int grid_size = grid.size());

        void train(const std::vector<std::pair<std::vector<float>, std::vector<float>>> &dataset, \
        int num_epoch = 1000, float lr = .1);

        std::vector<float> predict(const std::vector<float> &x);

        void updateAll(float lr, int epoch_size);

        void saveParameters(const std::string& filename) const;

        void loadParameters(const std::string& filename);
    };
};

#endif