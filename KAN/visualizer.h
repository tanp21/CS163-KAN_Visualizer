#ifndef VISUALIZER_H
#define VISUALIZER_H

#define DATA_VIS_PATH "./resources/Visualizer/"

#include "../include/raylib.h"
#include "../EngineSupport.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cassert>

class SplineBoxVisualizer {
private:
    ButtonTexture box;
    const int BOX_SIZE = 65;
    float x_pos, y_pos;
    std::vector<float> x, y;
    const float min_x = -1, max_x = 1;
    float min_y, max_y;

public:
    SplineBoxVisualizer(const std::string&file_path, float x_pos, float y_pos)
        : box(file_path, x_pos, y_pos), x_pos(x_pos), y_pos(y_pos) {};

    void setGraph(const std::vector<float>& _x, const std::vector<float>& _y) {
        assert(_x.size() == _y.size());

        x = _x, y = _y;

        min_y = *std::min_element(y.begin(), y.end());
        max_y = *std::max_element(y.begin(), y.end());
    }

    void show() {
        box.draw();

        for (int i = 0; i+1 < x.size(); i++) {
            Vector2 pos1 = {
                x_pos + (x[i] - min_x)/(max_x - min_x)*BOX_SIZE,
                y_pos + BOX_SIZE - (y[i] - min_y)/(max_y - min_y)*BOX_SIZE
            };
            Vector2 pos2 = {
                x_pos + (x[i+1] - min_x)/(max_x - min_x)*BOX_SIZE,
                y_pos + BOX_SIZE - (y[i+1] - min_y)/(max_y - min_y)*BOX_SIZE
            };
            DrawLineEx(pos1, pos2, 2, BLACK);
            // DrawCircleV(screen_pos, 2, BLACK);
        }
    }

    bool action() {
        return box.action();
    }
};

class SplineVisualizer {
private:
    const float X_MIN = -1.0;
    const float X_MAX =  1.0;

    const Vector2 ORIGIN = {1920.0/2, 973.0/2};

    const int POS_LEFT = 960 - 744/2, POS_RIGHT = 960 + 744/2;

    float x_scale = 960 - POS_LEFT;
    float y_scale = 960 - POS_LEFT;

    AutoTexture background;

public:
    SplineVisualizer() 
        : background(DATA_VIS_PATH"box_large.png", 533, 47) {}

    void action() {
        float mouseWheelMove = GetMouseWheelMove();

        if (mouseWheelMove != 0) {
            float zoom_factor = 1.1f;
            if (mouseWheelMove > 0) {
                x_scale *= zoom_factor;
                y_scale *= zoom_factor;
            }
            else {
                x_scale /= zoom_factor;
                y_scale /= zoom_factor;
            }
        }
    }

    void show() {
        background.draw();

        // X and Y axes
        DrawLine(POS_LEFT, 973.0/2, POS_RIGHT, 973.0/2, DARKGRAY);
        DrawLine(960, 973.0/2 - 744/2, 960, 973.0/2 + 744/2, DARKGRAY);

        // for (float x = X_MIN; x <= X_MAX; x += 0.01f) {
        //     float y = KANN::silu(x);

        //     Vector2 screen_pos = {
        //         ORIGIN.x + x*x_scale,
        //         ORIGIN.y - y*y_scale
        //     };

        //     DrawCircleV(screen_pos, 2, RED);
        // }
    }
};

class DataVisualizer {
private:
    enum State {
        VIS_NETWORK,
        VIS_SPLINE
    };

    State state;

    const int SPLINE_BOX_W = 65;
    const int NEURON_W = 26;
    const int LEFT_NEURON = 100;
    const int RIGHT_NEURON = 1794;
    const int BOX_DIS = 21;

    std::vector<std::vector<ButtonTexture>> neuron;
    std::vector<std::vector<std::vector<std::vector<LineDrawer>>>> edges;
    std::vector<std::vector<std::vector<SplineBoxVisualizer>>> splines;

    SplineVisualizer spline_vis;

public:
    DataVisualizer() {}

    void init(const std::vector<int> &layer_in4) {
        assert((int)layer_in4.size() >= 1);
        state = VIS_NETWORK;

        int gapH = ((1920 - 200) - NEURON_W*(int)layer_in4.size())/((int)layer_in4.size()-1);

        neuron.resize(layer_in4.size());
        edges.resize((int)layer_in4.size() - 1);

        for (int l = 0, curX = 100; l < (int)layer_in4.size(); l++, curX += gapH + NEURON_W) {
            assert(layer_in4[l] >= 0);
            int gapV = (973 - NEURON_W*layer_in4[l])/(layer_in4[l] + 1);

            for (int j = 0, curY = gapV; j < layer_in4[l]; j++, curY += gapV + NEURON_W) {
                neuron[l].emplace_back(DATA_VIS_PATH"neuron.png", curX, curY);
            }
        }

        splines.resize((int)layer_in4.size() - 1);

        for (int l = 0; l + 1 < (int)layer_in4.size(); l++) {
            int block_V = SPLINE_BOX_W*layer_in4[l+1] + \
                BOX_DIS*(layer_in4[l+1] - 1);
            int gap_V = (973 - layer_in4[l]*block_V)/(layer_in4[l] + 1);
            int curX = 100 + (l+1)*NEURON_W + l*gapH + (gapH - SPLINE_BOX_W)/2;

            splines[l].resize(layer_in4[l]);
            edges[l].resize(layer_in4[l]);

            int neu_gapV = (973 - NEURON_W*layer_in4[l])/(layer_in4[l] + 1);
            int neu_gapV_nxt = (973 - NEURON_W*layer_in4[l+1])/(layer_in4[l+1] + 1);

            for (int i = 0, curY = gap_V, neu_y = neu_gapV; i < layer_in4[l]; i++, curY += gap_V + block_V, neu_y += neu_gapV + NEURON_W) {
                edges[l][i].resize(layer_in4[l+1]);

                for (int j = 0, neu_y_nxt = neu_gapV_nxt; j < layer_in4[l+1]; j++, neu_y_nxt += neu_gapV_nxt + NEURON_W) {
                    // edges[l][i].emplace_back(DATA_VIS_PATH"", );
                    int currY = curY + j*(SPLINE_BOX_W + BOX_DIS);
                    splines[l][i].emplace_back(DATA_VIS_PATH"box.png", curX, currY);
                    edges[l][i][j].emplace_back(curX - (gapH - SPLINE_BOX_W)/2,
                        neu_y + NEURON_W/2, curX, currY + SPLINE_BOX_W/2, LIGHTGRAY);
                    edges[l][i][j].emplace_back(curX + SPLINE_BOX_W,
                        currY + SPLINE_BOX_W/2, curX + SPLINE_BOX_W + (gapH - SPLINE_BOX_W)/2, 
                        neu_y_nxt + NEURON_W/2, LIGHTGRAY);
                }
            }
        }
    }

    void show() {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < neuron.size(); i++) {
            for (int j = 0; j < neuron[i].size(); j++) {
                neuron[i][j].draw();
            }
        }

        for (int l = 0; l + 1 < neuron.size(); l++) {
            for (int i = 0; i < neuron[l].size(); i++) {
                for (int j = 0; j < neuron[l+1].size(); j++) {
                    splines[l][i][j].action();
                    splines[l][i][j].show();
                    assert(edges[l][i][j].size() == 2);
                    for (int k = 0; k < 2; k++) {
                        edges[l][i][j][k].draw();
                    }
                }
            }
        }

        EndDrawing();

        // spline_vis.action();
        // spline_vis.show();
    }

    void setSpline(int lvl, int src, int snk, const std::vector<float> &x, const std::vector<float> &y) {
        splines[lvl][src][snk].setGraph(x, y);
    }
};

#endif // VISUALIZER_H