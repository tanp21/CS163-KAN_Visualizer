#include<bits/stdc++.h>

#include "utils.h"
#include "spline.h"
#include "kanlayer.h"
#include "kannet.h"

#ifdef LOCAL
#include "debug.hpp"
#else
#define debug(...) 42
#endif


const float PI = acos(-1);

float func(float x, float y) {
    return exp(sin(PI * x) + y*y);
}

float randomFloat(float l, float r) {
    // Normalize rand() to [0, 1]
    float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    
    // Scale to the range [l, r]
    float result = l + random * (r - l);
    
    return result;
}

void solve() {
    KANN::set_grid_range(-1, 1, 42);
    KANN::KAN kan({2, 1, 1});

    std::vector<std::pair<std::vector<float>, std::vector<float>>> dataset;
    for(int i = 0; i < 10000; i++) {
        float x = randomFloat(-1, 1);
        float y = randomFloat(-1, 1);

        // cout << x << ", " << y << " = " << func(x, y) << "\n";
        dataset.push_back({{x, y}, {func(x, y)}});
    }

    kan.train(dataset, 10000);

    // for(int i = 0; i < 10; i++) {
    //     float x = randomFloat(-1, 1);
    //     float y = randomFloat(-1, 1);
    //     cout << x << ", " << y << " = " << func(x, y) << ", " << kan.predict({x, y})[0] << "\n";
    // }
}

int main(int argc, char* argv[]) {
    srand(atoi(argv[1]));

    int tt = 1;
    // cin >> tt;
    for(int i = 1; i <= tt; i++) {
        // cout << "Case " << i << "#: ";
        solve();
    }

    return 0;
}