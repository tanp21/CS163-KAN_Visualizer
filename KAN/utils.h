#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include "visualizer.h"

namespace KANN {
    extern std::vector<float> grid;
    extern std::unique_ptr<DataVisualizer> visualizer;
    extern std::unique_ptr<DataVisualizer> visualizer_sbs;
    extern std::vector<DataVisualizer> animations;
    extern std::vector<int> prev_in4;
    extern std::vector<std::vector<float>> val_node;
    extern int cnt;
    // extern std::vector<DataVisualizer> animations;


    void set_grid_range(float L, float R, int sz);

    double sigmoid(double x);

    float silu(float x);

    std::vector<float> silu(const std::vector<float> &x);

    float silu_derivative(float x);

    std::vector<float> linspace(float L, float R, int steps);
};
#endif