//
// Created by tonib on 14.12.2025.
//

#include "Calculations.h"

#include <stdlib.h>
#include <util/pcgsolver.h>

float Calculations::calc_delta_x(std::vector<std::vector<float> > domain, float n) {
    float x_length = domain[0][1] - domain[0][0];
    return x_length / (n + 2);
}

float Calculations::calc_delta_y(std::vector<std::vector<float> > domain, float m) {
    float y_length = domain[1][1] - domain[1][0];
    return y_length / (m + 2);
}

std::vector<std::vector<float> > Calculations::calc_explicit_euler(float timestep, std::vector<std::vector<float>> temperatureField, float thermalDiffusivity,  float m, float n, float delta_x, float delta_y) {
    std::vector<std::vector<float>> out(m + 2, std::vector<float>(n + 2, 0.0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            out[i][j] = thermalDiffusivity * (1/(delta_x * delta_x) * (temperatureField[i+1][j] - 2*temperatureField[i][j] + temperatureField[i-1][j]) + 1/(delta_y * delta_y) * (temperatureField[i][j+1] - 2*temperatureField[i][j] + temperatureField[i][j-1]));
            out[i][j] *= timestep;
            out[i][j] += temperatureField[i][j];
        }
    }
    return out;
}

std::vector<std::vector<float> > Calculations::calc_implicit_euler(float timestep, std::vector<std::vector<float> > temperatureField, float thermalDiffusivity, float m, float n, float delta_x, float delta_y) {
    float K1 = (1 + 2*((thermalDiffusivity * timestep) / (delta_x * delta_x)) + 2*((thermalDiffusivity * timestep) / (delta_y * delta_y)));
    float K2 = -1 * ((thermalDiffusivity * timestep) / (delta_x * delta_x));
    float K3 = -1 * ((thermalDiffusivity * timestep) / (delta_y * delta_y));

    std::vector<float> b = {};
    for (const auto &row : temperatureField) {
        for (float element : row) {
            b.push_back(element);
        }
    }

    SparseMatrix<float> A(b.size(), b.size() - 5);
    //std::vector<std::vector<float>> A(b.size(), std::vector<float>(b.size(), 0));
    for (int i = 0; i < b.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            //Todo: mach A

        }
    }

    SparsePCGSolver<float> solver();
}


std::vector<std::vector<float> > Calculations::initializeTemperatureField(float m, float n) {
    std::vector<std::vector<float>> out(m + 2, std::vector<float>(n + 2, 0.0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            out[i][j] = rand();
        }
    }
    return out;
}
