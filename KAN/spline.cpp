#ifndef SPLINE_CPP
#define SPLINE_CPP

#include "utils.h"
#include "spline.h"
#include "visualizer.h"

namespace KANN {
    const std::vector<float>& B_Spline::getBases() const { return bases; }
    const std::vector<float>& B_Spline::getCoef() const { return coef; }
    const std::vector<float>& B_Spline::getCoefGrad() const { return coef_grad; }
    float B_Spline::getDbDx() const { return db_dx; }
    float B_Spline::getLast() const { return last; }
    int B_Spline::getSplineOrder() const { return spline_order; }
    int B_Spline::getNumPoints() const { return num_points; }
    int B_Spline::getNumBases() const { return num_bases; }

    void B_Spline::setBases(const std::vector<float>& bases) { this->bases = bases; }
    void B_Spline::setCoef(const std::vector<float>& coef) { this->coef = coef; }
    void B_Spline::setCoefGrad(const std::vector<float>& coefGrad) { this->coef_grad = coefGrad; }
    void B_Spline::setDbDx(float dbDx) { this->db_dx = dbDx; }
    void B_Spline::setLast(float last) { this->last = last; }

    B_Spline::B_Spline(int spline_order, int num_points) : 
    spline_order(spline_order),
    num_points(num_points),
    num_bases(num_points - spline_order - 1)
    {
        bases.resize((int)grid.size() - 1);
        coef = std::vector<float>(num_bases);
        coef_grad = std::vector<float>(num_bases);
    }

    void B_Spline::cox_de_boor(float x) {
        std::vector<float> pre;
        bases.resize((int)grid.size() - 1);
        for (int i = 0; i + 1 < grid.size(); i++) {
            bases[i] = (grid[i] <= x && x < grid[i+1]);
        }

        pre = bases;

        for (int k = 1; k <= spline_order; k++) {
            bases.resize((int)grid.size() - k - 1);
            for (int i = 0; i + k + 1 < grid.size(); i++) {
                bases[i] = (x - grid[i])/(grid[i + k] - grid[i])*pre[i] + \
                    (grid[i+k+1] - x)/(grid[i+k+1] - grid[i+1])*pre[i+1];
            }

            db_dx = 0;
            if(k == spline_order) {
                // calc derivative
                for (int i = 0; i + k + 1 < grid.size(); i++) {
                    db_dx += coef[i]*(pre[i]/(grid[i + k] - grid[i]) + \
                        pre[i+1]/(grid[i+k+1] - grid[i+1]));
                }
                db_dx *= k;
            }

            pre = bases;
        }

    }

    float B_Spline::derivative() {
        return db_dx;
    }

    float B_Spline::eval(float x) {
        assert(coef.size() == bases.size());
        float res = 0;
        for(int i = 0; i < bases.size(); i++) {
            res += coef[i]*bases[i];
        }
        return last = res;
    }

    float B_Spline::eval_tmp(float x) {
        std::vector<float> pre, bases_tmp((int)grid.size() - 1);
        // bases.resize((int)grid.size() - 1);
        for (int i = 0; i + 1 < grid.size(); i++) {
            bases_tmp[i] = (grid[i] <= x && x < grid[i+1]);
        }

        pre = bases_tmp;

        for (int k = 1; k <= spline_order; k++) {
            bases_tmp.resize((int)grid.size() - k - 1);
            for (int i = 0; i + k + 1 < grid.size(); i++) {
                bases_tmp[i] = (x - grid[i])/(grid[i + k] - grid[i])*pre[i] + \
                    (grid[i+k+1] - x)/(grid[i+k+1] - grid[i+1])*pre[i+1];
            }

            pre = bases_tmp;
        }

        float res = 0.;
        for (int i = 0; i < bases_tmp.size(); i++) {
            res += bases_tmp[i]*coef[i];
        }

        return res;
    }

    std::vector<float> B_Spline::eval_tmp(const std::vector<float> &x) {
        std::vector<float> y(x.size());
        for (int i = 0; i < y.size(); i++) {
            y[i] = B_Spline::eval_tmp(x[i]);
        }
        return y;
    }

    float B_Spline::get_last() const {
        return last;
    }

    void B_Spline::backward(float last_d) {
        for (int i = 0; i < bases.size(); i++) {
            coef_grad[i] += bases[i] * last_d;
        }
    }

    void B_Spline::updateAll(float lr, int epoch_size) {
        for (int i = 0; i < bases.size(); i++) {
            coef[i] -= lr * coef_grad[i] / epoch_size;
            coef_grad[i] = 0;
        }
    }
};
#endif