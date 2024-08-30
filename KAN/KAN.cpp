#ifndef KANNET_CPP
#define KANNET_CPP

#include "KAN.h"
#include "visualizer.h"
#include <string>

namespace KANN {
    const std::vector<KANLayer>& KAN::getLayers() const { return layers; }

    std::vector<float> KAN::forward(std::vector<float> x) {
        for (int i = 0; i < layers.size(); i++) {
            x = layers[i].forward_from(x);
        }
        return x;
    }

    void KAN::back_propagation(const std::vector<float> &y) {
        std::vector<float> grad = layers.back().calc_base_grad(y);
        for(int i = layers.size() - 1; ~i; i--) {
            grad = layers[i].backward(grad);
        }
    }

    KAN::KAN(const std::vector<int> &layer_in4, int spline_order, int grid_size) {
        // visualizer.init(layer_in4);
        visualizer = std::make_unique<DataVisualizer>();
        visualizer->init(layer_in4);
        layers.clear();
        for (int i = 1; i < layer_in4.size(); i++) {
            layers.push_back(KANLayer(layer_in4[i-1], layer_in4[i], spline_order, i-1));
        }
    }


    void KAN::train(const std::vector<std::pair<std::vector<float>, std::vector<float>>> &dataset, \
    int num_epoch, float lr) {
        float prev_loss = 1e6;
        const float EPS = 1e-6;
        int epoch_size = dataset.size();
        for (int i = 0; i < num_epoch; i++) {
            float loss = 0;
            for(auto &[x, y] : dataset) {
                forward(x);
                loss += layers.back().loss_func(y);
                back_propagation(y);
            }
            loss /= epoch_size;
            if(loss > prev_loss || abs(loss - prev_loss) <= EPS) {
                std::cerr << "DONE\n";
                return;
            }
            prev_loss = loss;
            updateAll(lr, epoch_size);
            std::cerr << "LOSS = " << loss << "\n";
        }
        std::cerr << "DONE\n";
    }

    std::vector<float> KAN::predict(const std::vector<float> &x) {
        return forward(x);
    }

    void KAN::updateAll(float lr, int epoch_size) {
        for (int i = (int)layers.size()-1; ~i; i--) {
            layers[i].updateAll(lr, epoch_size);
        }
        saveParameters("history/" + std::to_string(cnt) + ".bin");
        cnt++;
    }

    void KAN::saveParameters(const std::string& filename) const {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        // Retrieve the number of layers
        int numLayers = layers.size();
        outFile.write(reinterpret_cast<const char*>(&numLayers), sizeof(numLayers));

        for (const auto& layer : layers) {
            // Save layer parameters
            int numFeatures = layer.getNumFeatures();
            int numIn = layer.getNumIn();
            int numOut = layer.getNumOut();
            int splineOrder = layer.getSplineOrder();
            int level = layer.getLevel();

            outFile.write(reinterpret_cast<const char*>(&numFeatures), sizeof(numFeatures));
            outFile.write(reinterpret_cast<const char*>(&numIn), sizeof(numIn));
            outFile.write(reinterpret_cast<const char*>(&numOut), sizeof(numOut));
            outFile.write(reinterpret_cast<const char*>(&splineOrder), sizeof(splineOrder));
            outFile.write(reinterpret_cast<const char*>(&level), sizeof(level));

            // Save bias and bias_grad
            const auto& bias = layer.getBias();
            const auto& biasGrad = layer.getBiasGrad();
            outFile.write(reinterpret_cast<const char*>(bias.data()), bias.size() * sizeof(float));
            outFile.write(reinterpret_cast<const char*>(biasGrad.data()), biasGrad.size() * sizeof(float));

            // Save scale_spline and scale_spline_grad
            const auto& scaleSpline = layer.getScaleSpline();
            const auto& scaleSplineGrad = layer.getScaleSplineGrad();
            for (const auto& vec : scaleSpline) {
                outFile.write(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(float));
            }
            for (const auto& vec : scaleSplineGrad) {
                outFile.write(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(float));
            }

            // Save scale_basis and scale_basis_grad
            const auto& scaleBasis = layer.getScaleBasis();
            const auto& scaleBasisGrad = layer.getScaleBasisGrad();
            for (const auto& vec : scaleBasis) {
                outFile.write(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(float));
            }
            for (const auto& vec : scaleBasisGrad) {
                outFile.write(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(float));
            }

            // Save the B_Spline objects
            const auto& splines = layer.getSpline();
            for (const auto& splinesRow : splines) {
                for (const auto& spline : splinesRow) {
                    // Save B_Spline specific data
                    int splineOrder = spline.getSplineOrder();
                    int numPoints = spline.getNumPoints();
                    int numBases = spline.getNumBases();

                    outFile.write(reinterpret_cast<const char*>(&splineOrder), sizeof(splineOrder));
                    outFile.write(reinterpret_cast<const char*>(&numPoints), sizeof(numPoints));
                    outFile.write(reinterpret_cast<const char*>(&numBases), sizeof(numBases));

                    // Save vectors: bases, coef, coef_grad
                    const auto& bases = spline.getBases();
                    const auto& coef = spline.getCoef();
                    const auto& coefGrad = spline.getCoefGrad();
                    outFile.write(reinterpret_cast<const char*>(bases.data()), bases.size() * sizeof(float));
                    outFile.write(reinterpret_cast<const char*>(coef.data()), coef.size() * sizeof(float));
                    outFile.write(reinterpret_cast<const char*>(coefGrad.data()), coefGrad.size() * sizeof(float));

                    // Save floats: db_dx, last
                    float dbDx = spline.getDbDx();
                    float last = spline.getLast();
                    outFile.write(reinterpret_cast<const char*>(&dbDx), sizeof(dbDx));
                    outFile.write(reinterpret_cast<const char*>(&last), sizeof(last));
                }
            }
        }

        outFile.close();
        if (outFile.good()) {
            std::cout << "Parameters successfully saved to " << filename << std::endl;
        } else {
            std::cerr << "Error occurred while saving parameters to " << filename << std::endl;
        }
    }

    void KAN::loadParameters(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            std::cerr << "Failed to open file for reading: " << filename << std::endl;
            return;
        }

        // Read the number of layers
        int numLayers;
        inFile.read(reinterpret_cast<char*>(&numLayers), sizeof(numLayers));
        // layers.resize(numLayers);
        layers.clear();

        visualizer_sbs = std::make_unique<DataVisualizer>();
        visualizer_sbs->init(prev_in4);

        for (int i = 0; i < numLayers; i++) {
            // Read layer parameters
            int numFeatures, numIn, numOut, splineOrder, level;
            inFile.read(reinterpret_cast<char*>(&numFeatures), sizeof(numFeatures));
            inFile.read(reinterpret_cast<char*>(&numIn), sizeof(numIn));
            inFile.read(reinterpret_cast<char*>(&numOut), sizeof(numOut));
            inFile.read(reinterpret_cast<char*>(&splineOrder), sizeof(splineOrder));
            inFile.read(reinterpret_cast<char*>(&level), sizeof(level));

            KANLayer layer(numIn, numOut, splineOrder, level);


            // Read bias and bias_grad
            std::vector<float> bias(numOut), biasGrad(numOut);
            inFile.read(reinterpret_cast<char*>(bias.data()), numOut * sizeof(float));
            inFile.read(reinterpret_cast<char*>(biasGrad.data()), numOut * sizeof(float));
            layer.setBias(bias);
            layer.setBiasGrad(biasGrad);

            // Read scale_spline and scale_spline_grad
            std::vector<std::vector<float>> scaleSpline(numOut, std::vector<float>(numIn));
            std::vector<std::vector<float>> scaleSplineGrad(numOut, std::vector<float>(numIn));
            for (auto& vec : scaleSpline) {
                inFile.read(reinterpret_cast<char*>(vec.data()), numIn * sizeof(float));
            }
            for (auto& vec : scaleSplineGrad) {
                inFile.read(reinterpret_cast<char*>(vec.data()), numIn * sizeof(float));
            }
            layer.setScaleSpline(scaleSpline);
            layer.setScaleSplineGrad(scaleSplineGrad);

            // Read scale_basis and scale_basis_grad
            std::vector<std::vector<float>> scaleBasis(numOut, std::vector<float>(numIn));
            std::vector<std::vector<float>> scaleBasisGrad(numOut, std::vector<float>(numIn));
            for (auto& vec : scaleBasis) {
                inFile.read(reinterpret_cast<char*>(vec.data()), numIn * sizeof(float));
            }
            for (auto& vec : scaleBasisGrad) {
                inFile.read(reinterpret_cast<char*>(vec.data()), numIn * sizeof(float));
            }
            layer.setScaleBasis(scaleBasis);
            layer.setScaleBasisGrad(scaleBasisGrad);

            // Read the B_Spline objects
            std::vector<std::vector<B_Spline>> splines(numOut, std::vector<B_Spline>(numIn));
            for (auto& splinesRow : splines) {
                for (auto& spline : splinesRow) {
                    // Read B_Spline specific data
                    int splineOrder, numPoints, numBases;
                    inFile.read(reinterpret_cast<char*>(&splineOrder), sizeof(splineOrder));
                    inFile.read(reinterpret_cast<char*>(&numPoints), sizeof(numPoints));
                    inFile.read(reinterpret_cast<char*>(&numBases), sizeof(numBases));

                    spline = B_Spline(splineOrder, numPoints);

                    // Read vectors: bases, coef, coef_grad
                    std::vector<float> bases(numBases);
                    std::vector<float> coef(numBases);
                    std::vector<float> coefGrad(numBases);
                    inFile.read(reinterpret_cast<char*>(bases.data()), bases.size() * sizeof(float));
                    inFile.read(reinterpret_cast<char*>(coef.data()), coef.size() * sizeof(float));
                    inFile.read(reinterpret_cast<char*>(coefGrad.data()), coefGrad.size() * sizeof(float));
                    spline.setBases(bases);
                    spline.setCoef(coef);
                    spline.setCoefGrad(coefGrad);

                    // Read floats: db_dx, last
                    float dbDx, last;
                    inFile.read(reinterpret_cast<char*>(&dbDx), sizeof(dbDx));
                    inFile.read(reinterpret_cast<char*>(&last), sizeof(last));
                    spline.setDbDx(dbDx);
                    spline.setLast(last);
                }
            }
            layer.setSplines(splines);
            layers.push_back(layer);
        }

        inFile.close();
        if (inFile.good()) {
            std::cout << "Parameters successfully loaded from " << filename << std::endl;
        } else {
            std::cerr << "Error occurred while loading parameters from " << filename << std::endl;
        }

        visualizer_sbs->show(false);
    }
};

#endif