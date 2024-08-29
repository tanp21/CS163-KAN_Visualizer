#ifndef SPLINE_H
#define SPLINE_H

#include <bits/stdc++.h>
#include "utils.h"

namespace KANN {
    class B_Spline {
        private:

        std::vector<float> bases;
        std::vector<float> coef;
        std::vector<float> coef_grad;

        float db_dx;
        float last;

        int spline_order;
        int num_points;
        int num_bases;

        public:

        B_Spline(int spline_order = 3, int num_points = grid.size());

        void cox_de_boor(float x);

        float derivative();

        float eval(float x);
        float eval_tmp(float x);
        std::vector<float> eval_tmp(const std::vector<float> &x);

        float get_last() const;

        void backward(float last_d);

        void updateAll(float lr, int epoch_size);
    };
};
#endif