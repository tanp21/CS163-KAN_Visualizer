#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>

namespace KANN {
    std::vector<float> grid;

    void set_grid_range(float L, float R, int sz);

    double sigmoid(double x);

    float silu(float x);

    float silu_derivative(float x);
};
#endif