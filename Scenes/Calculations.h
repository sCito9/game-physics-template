//
// Created by tonib on 14.12.2025.
//

#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include <vector>


class Calculations {
    public:
    static float calc_delta_x(std::vector<std::vector<float>> domain, int n);
    static float calc_delta_y(std::vector<std::vector<float>> domain, int m);
    static std::vector<std::vector<float>> calc_explicit_euler(float timestep, std::vector<std::vector<float>> temperatureField, float thermalDiffusivity, int m, int n, float delta_x, float delta_y);
    static std::vector<std::vector<float>> calc_implicit_euler(float timestep, std::vector<std::vector<float>> temperatureField, float thermalDiffusivity, int m, int n, float delta_x, float delta_y);
    static std::vector<std::vector<float>> initializeTemperatureField(int m, int n);
    static std::vector<std::vector<float>> resizeTemperatureField(std::vector<std::vector<float>> oldTemperatureField, int new_m, int new_n);
    static std::vector<std::vector<float>> coolerResizeTemperatureField(std::vector<std::vector<float> > oldTemperatureField, int new_m, int new_n, bool* variety);
};



#endif //CALCULATIONS_H
