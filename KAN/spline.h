#ifndef SPLINE_H
#define SPLINE_H

#include <bits/stdc++.h>
#include "utils.h"
#include "visualizer.h"

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

        const std::vector<float>& getBases() const;
        const std::vector<float>& getCoef() const;
        const std::vector<float>& getCoefGrad() const;
        float getDbDx() const;
        float getLast() const;
        int getSplineOrder() const;
        int getNumPoints() const;
        int getNumBases() const;

        void setBases(const std::vector<float>& bases);
        void setCoef(const std::vector<float>& coef);
        void setCoefGrad(const std::vector<float>& coefGrad);
        void setDbDx(float dbDx);
        void setLast(float last);

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