#ifndef KANLAYER_H
#define KANLAYER_H

#include <bits/stdc++.h>
#include "utils.h"
#include "spline.h"

namespace KANN {
    class KANLayer {
        private:

        std::vector<std::vector<float>> scale_spline;
        std::vector<std::vector<float>> scale_spline_grad;
        std::vector<std::vector<float>> scale_basis;
        std::vector<std::vector<float>> scale_basis_grad;

        std::vector<float> x;
        std::vector<float> bias;
        std::vector<float> bias_grad;
        std::vector<float> last_silu;
        std::vector<float> silu_d;

        std::vector<std::vector<B_Spline>> spline;

        int num_features;
        int num_bases;
        int spline_order;
        int num_in, num_out;

        public:

        KANLayer(int num_in, int num_out, int spline_order);

        std::vector<float> forward_from(const std::vector<float> &prev_x);
        std::vector<float> backward(const std::vector<float> &next_grad);
        std::vector<float> calc_base_grad(const std::vector<float> &y);

        void updateAll(float lr, int epoch_size);

        float loss_func(const std::vector<float> &y);
    };
};

#endif