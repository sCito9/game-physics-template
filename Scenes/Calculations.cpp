//
// Created by tonib on 14.12.2025.
//

#include "Calculations.h"

#include <iostream>
#include <stdlib.h>
#include <util/pcgsolver.h>

float Calculations::calc_delta_x(std::vector<std::vector<float> > domain, int n) {
    float x_length = domain[0][1] - domain[0][0];
    return x_length / (n + 1);
}

float Calculations::calc_delta_y(std::vector<std::vector<float> > domain, int m) {
    float y_length = domain[1][1] - domain[1][0];
    return y_length / (m + 1);
}

std::vector<std::vector<float> > Calculations::calc_explicit_euler(float timestep, std::vector<std::vector<float>> temperatureField, float thermalDiffusivity,  int m, int n, float delta_x, float delta_y) {
    std::vector<std::vector<float>> out(m + 2, std::vector<float>(n + 2, 0.0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            out[i][j] = thermalDiffusivity * (1/(delta_x * delta_x) * (temperatureField[i+1][j] - 2*temperatureField[i][j]+ temperatureField[i-1][j])
                                            + 1/(delta_y * delta_y) * (temperatureField[i][j+1] - 2*temperatureField[i][j]+ temperatureField[i][j-1]));
            out[i][j] *= timestep;
            out[i][j] += temperatureField[i][j];
        }
    }
    return out;
}

std::vector<std::vector<float> > Calculations::calc_implicit_euler(float timestep, std::vector<std::vector<float> > temperatureField, float thermalDiffusivity, int m, int n, float delta_x, float delta_y) {
    float K1 = (1 + 2*((thermalDiffusivity * timestep) / (delta_x * delta_x)) + 2*((thermalDiffusivity * timestep) / (delta_y * delta_y)));
    float K2 = -1 * ((thermalDiffusivity * timestep) / (delta_x * delta_x));
    float K3 = -1 * ((thermalDiffusivity * timestep) / (delta_y * delta_y));

    std::vector<std::vector<float>> out(m + 2, std::vector<float>(n + 2, 0.0));

    std::vector<float> b(temperatureField.size() * temperatureField[0].size(), 0);
    std::vector<float> x(b.size(), 0);

    SparseMatrix<float> A(b.size(), 5);

    float size = temperatureField[0].size();
    for (int i = 0; i < temperatureField.size(); i++) {
        for (int j = 0; j < temperatureField[i].size(); j++) {
            b.at(temperatureField[i].size()*i + j) = temperatureField[i][j];

            if (i==0 || i == temperatureField.size()-1 || j == 0 || j == temperatureField[i].size() - 1) {
                A.set_element(size*i + j, size*i + j, 1.f);
                continue;
            }

            A.set_element(size*i + j, size*i + j, K1);
            A.set_element(size*i + j, size*(i+1) + j, K2);
            A.set_element(size*i + j, size*(i-1) + j, K2);
            A.set_element(size*i + j, size*i + j+1, K3);
            A.set_element(size*i + j, size*i + j-1, K3);
        }
    }
    SparsePCGSolver<float> solver = SparsePCGSolver<float>();
    float relative_residual;
    int iterations;

    bool success = solver.solve(A, b, x, relative_residual, iterations);
    if (success) {
        for (int i = 0; i < out.size(); i++) {
            for (int j = 0; j < out[i].size(); j++) {
                out[i][j] = x.at(temperatureField[i].size()*i + j);
            }
        }
        return out;
    }
    else {
        std::cout << "Failed to converge within the maximum iterations.\n" << std::endl;
        for (int i = 0; i < out.size(); i++) {
            for (int j = 0; j < out[i].size(); j++) {
                out[i][j] = x.at(temperatureField[i].size()*i + j);
            }
        }
        return out;
    }
}


std::vector<std::vector<float> > Calculations::initializeTemperatureField(int m, int n) {
    std::vector<std::vector<float>> out(m + 2, std::vector<float>(n + 2, 0.0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            out[i][j] = rand();
        }
    }
    return out;
}

std::vector<std::vector<float> > Calculations::resizeTemperatureField(std::vector<std::vector<float> > oldTemperatureField, int new_m, int new_n) {
    std::vector<std::vector<float>> out(new_m + 2, std::vector<float>(new_n + 2, 0.0));
    float old_m = oldTemperatureField.size()-2;
    float old_n = oldTemperatureField[0].size()-2;

    if (new_m > old_m) {
        for (int i = 0; i < oldTemperatureField.size(); i++) {
            if (new_n > old_n) {
                for (int j = 0; j < oldTemperatureField[i].size(); j++) {
                    out[i][j] = oldTemperatureField[i][j];
                }
            }
            else {
                for (int j = 0; j < out[i].size(); j++) {
                    out[i][j] = oldTemperatureField[i][j];
                }
            }
        }
    }
    else {
        for (int i = 0; i < out.size(); i++) {
            if (new_n > old_n) {
                for (int j = 0; j < oldTemperatureField[i].size(); j++) {
                    out[i][j] = oldTemperatureField[i][j];
                }
            }
            else {
                for (int j = 0; j < out[i].size(); j++) {
                    out[i][j] = oldTemperatureField[i][j];
                }
            }
        }
    }

    return out;
}

std::vector<std::vector<float> > Calculations::coolerResizeTemperatureField(std::vector<std::vector<float> > oldTemperatureField, int new_m, int new_n, bool* variety) {
    std::vector<std::vector<float>> out(new_m + 2, std::vector<float>(new_n + 2, 0.0));
    float old_m = oldTemperatureField.size()-2;
    float old_n = oldTemperatureField[0].size()-2;
    int difference_m = new_m - old_m;
    int difference_n = new_n - old_n;
    int shift_m;
    int shift_n;
    if (*variety) { //Todo: doesn't work
        shift_m = difference_m / 2;
        shift_n = difference_n / 2;
    }
    else {
        shift_m = difference_m/2 + 1;
        shift_n = difference_n/2 + 1;
    }
    *variety = !*variety;

    if (new_m > old_m) {
        for (int i = 0; i < oldTemperatureField.size(); i++) {
            if (new_n > old_n) {
                for (int j = 0; j < oldTemperatureField[i].size(); j++) {
                    out[i + shift_m][j + shift_n] = oldTemperatureField[i][j];
                }
            }
            else {
                for (int j = 0; j < out[i].size(); j++) {
                    out[i + shift_m][j] = oldTemperatureField[i][j - shift_n];
                }
            }
        }
    }
    else {
        for (int i = 0; i < out.size(); i++) {
            if (new_n > old_n) {
                for (int j = 0; j < oldTemperatureField[i].size(); j++) {
                    out[i][j + shift_n] = oldTemperatureField[i - shift_m][j];
                }
            }
            else {
                for (int j = 0; j < out[i].size(); j++) {
                    out[i][j] = oldTemperatureField[i - shift_m][j - shift_n];
                }
            }
        }
    }

    return out;
}
