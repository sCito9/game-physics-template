//
// Created by tonib on 14.12.2025.
//

#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include <vector>


class Calculations {
    public:
    static float calc_delta_x(std::vector<std::vector<float>> domain, float n);
    static float calc_delta_y(std::vector<std::vector<float>> domain, float m);
    static std::vector<std::vector<float>> calc_explicit_euler(float timestep, std::vector<std::vector<float>> temperatureField, float thermalDiffusivity, float m, float n, float delta_x, float delta_y);
    static std::vector<std::vector<float>> calc_implicit_euler(float timestep, std::vector<std::vector<float>> temperatureField, float thermalDiffusivity, float m, float n, float delta_x, float delta_y);
    static std::vector<std::vector<float>> initializeTemperatureField(float m, float n);
};



#endif //CALCULATIONS_H
