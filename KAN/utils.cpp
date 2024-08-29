#ifndef UTILS_CPP
#define UTILS_CPP

#include "utils.h"
#include <bits/stdc++.h>
#include "visualizer.h"

namespace KANN {
    std::vector<float> grid;  // Define the global variable here
    std::unique_ptr<DataVisualizer> visualizer;
    std::vector<DataVisualizer> animations;
    int cnt = 0;
    // std::vector<DataVisualizer> animations;

    // Define the functions here
    void set_grid_range(float L, float R, int sz) {
        sz = sz + 2 * 3 + 1;

        float bucket_size = (R - L) / sz;
        grid = {L - 3 * bucket_size};
        for (int i = 1; i < sz; i++) {
            grid.push_back(grid.back() + bucket_size);
        }
    }

    double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    float silu(float x) {
        if (x > 0)
            return x / (1 + expf(-x));
        else {
            float t = expf(x);
            return x * t / (1 + t);
        }
    }

    std::vector<float> silu(const std::vector<float> &x) {
        std::vector<float> y(x.size());
        for (int i = 0; i < x.size(); i++) {
            y[i] = silu(x[i]);
        }
        return y;
    }

    float silu_derivative(float x) {
        float sigmoid;

        if (x > 0)
            sigmoid = 1 / (1 + expf(-x));
        else {
            float t = expf(x);
            sigmoid = t / (1 + t);
        }

        return sigmoid * (1 + x * (1 - sigmoid));
    }

    std::vector<float> linspace(float L, float R, int steps) {
        std::vector<float> res(steps); 
        res[0] = L;
        float step_size = (R-L)/(steps - 1);
        for (int i = 0; i < steps-1; i++) {
            res[i+1] = res[i] + step_size;
        }
        return res;
    }
};
#endif