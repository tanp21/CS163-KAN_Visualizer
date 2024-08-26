#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>

namespace KANN {
    std::vector<float> grid;

    void set_grid_range(float L, float R, int sz) {
        sz = sz + 2*3 + 1;
    
    
        float bucket_size = (R - L)/sz;
        grid = {L - 3*bucket_size};
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
};
#endif