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
        int lvl;

        public:
        const std::vector<std::vector<float>>& getScaleSpline() const;
        const std::vector<std::vector<float>>& getScaleSplineGrad() const;
        const std::vector<std::vector<float>>& getScaleBasis() const;
        const std::vector<std::vector<float>>& getScaleBasisGrad() const;
        const std::vector<float>& getBias() const;
        const std::vector<float>& getBiasGrad() const;
        const std::vector<std::vector<B_Spline>>& getSpline() const;
        int getNumFeatures() const;
        int getNumIn() const;
        int getNumOut() const;
        int getSplineOrder() const;
        int getLevel() const;

        void setBias(const std::vector<float>& bias);
        void setBiasGrad(const std::vector<float>& biasGrad);
        void setScaleSpline(const std::vector<std::vector<float>>& scaleSpline);
        void setScaleSplineGrad(const std::vector<std::vector<float>>& scaleSplineGrad);
        void setScaleBasis(const std::vector<std::vector<float>>& scaleBasis);
        void setScaleBasisGrad(const std::vector<std::vector<float>>& scaleBasisGrad);
        void setSplines(const std::vector<std::vector<B_Spline>>& splines);

        KANLayer(int num_in, int num_out, int spline_order, int lvl);

        std::vector<float> forward_from(const std::vector<float> &prev_x);
        std::vector<float> backward(const std::vector<float> &next_grad);
        std::vector<float> calc_base_grad(const std::vector<float> &y);

        void updateAll(float lr, int epoch_size);

        float loss_func(const std::vector<float> &y);
    };
};

#endif