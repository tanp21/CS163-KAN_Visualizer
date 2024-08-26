#ifndef KANLAYER_CPP
#define KANLAYER_CPP

#include "spline.h"
#include "KANLayer.h"

namespace KANN {
    KANLayer::KANLayer(int num_in, int num_out, int spline_order) : 
        num_features(num_in),
        num_in(num_in),
        num_out(num_out),
        spline_order(spline_order)
    {
        spline = std::vector<std::vector<B_Spline>>(num_out, std::vector<B_Spline>(num_in));

        scale_spline = scale_basis = std::vector<std::vector<float>>(num_out, std::vector<float>(num_in, 1.0));
        scale_spline_grad = scale_basis_grad = std::vector<std::vector<float>>(num_out, std::vector<float>(num_in, 0.0));

        last_silu.resize(num_in);
        silu_d.resize(num_in);

        x.assign(num_out, 0);
        bias.assign(num_out, 0);
        bias_grad = bias;
    }

    std::vector<float> KANLayer::forward_from(const std::vector<float> &prev_x) {
        x.assign(num_out, 0.0);

        for (int j = 0; j < num_in; j++) {
            float _x = prev_x[j];
            last_silu[j] = silu(_x);
            silu_d[j] = silu_derivative(_x);
            for(int i = 0; i < num_out; i++) {
                spline[i][j].cox_de_boor(_x);
                x[i] += scale_spline[i][j]*spline[i][j].eval(_x) + scale_basis[i][j]*last_silu[j];
            }
        }

        for (int i = 0; i < x.size(); i++) {
            x[i] += bias[i];
        }

        return x;
    }

    std::vector<float> KANLayer::backward(const std::vector<float> &next_grad) {
        assert(next_grad.size() == bias_grad.size());
        for (int i = 0; i < bias_grad.size(); i++) {
            bias_grad[i] += next_grad[i];
        }

        std::vector<float> grad(num_in);

        for (int i = 0; i < num_out; i++) {
            for (int j = 0; j < num_in; j++) {
                spline[i][j].backward(scale_spline[i][j] * next_grad[i]);
                scale_spline_grad[i][j] += spline[i][j].get_last() * next_grad[i];
                scale_basis_grad[i][j] += last_silu[j] * next_grad[i];
            }
        }

        for (int j = 0; j < num_in; j++) {
            for (int i = 0; i < num_out; i++) {
                grad[j] += (spline[i][j].derivative()*scale_spline[i][j] + \
                    silu_d[j]*scale_basis[i][j]) * next_grad[i];
            }
        }

        return grad;
    }

    void KANLayer::updateAll(float lr, int epoch_size) {
        for (int i = 0; i < num_out; i++) {
            bias[i] -= lr * bias_grad[i] / epoch_size;
            bias_grad[i] = 0;
            for (int j = 0; j < num_in; j++) {
                spline[i][j].updateAll(lr, epoch_size);

                scale_spline[i][j] -= lr * scale_spline_grad[i][j] / epoch_size;
                scale_basis[i][j] -= lr * scale_basis_grad[i][j] / epoch_size;

                scale_spline_grad[i][j] = 0;
                scale_basis_grad[i][j] = 0;
            }
        }
    }

    std::vector<float> KANLayer::calc_base_grad(const std::vector<float> &y) {
        assert(y.size() == x.size());
        std::vector<float> res(y.size());
        for (int i = 0; i < y.size(); i++) {
            res[i] = 2.0*(x[i] - y[i]);
        }
        return res;
    }

    float KANLayer::loss_func(const std::vector<float> &y) {
        assert(x.size() == y.size());
        float res = 0;
        for (int i = 0; i < y.size(); i++) {
            res += (x[i] - y[i])*(x[i] - y[i]);
        }
        return res;
    }
};
#endif