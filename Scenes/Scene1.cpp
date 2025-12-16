//
// Created by tonib on 14.12.2025.
//

#include "Scene1.h"
#include "Calculations.h"
#include "imgui.h"

void Scene1::init() {
    domain = {{0, 2}, {0, 4}};
    m = 3;
    n = 6;
    temperatureField = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 6, 5, 1, -1, -2, -1, 0},
        {0, 4, 3, 0, -1, -3, -1, 0},
        {0, 3, 2, -1, -2, -4, -2, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    thermalDiffusivity = 0.1f;
    timeStep = 0.1f;

    delta_x = Calculations::calc_delta_x(domain, n);
    delta_y = Calculations::calc_delta_y(domain, m);

    std::cout << "T_1,3:  " << temperatureField[1+1][3+1] << "\nT_0,3: " << temperatureField[0+1][3+1] << "\nT_0,5: " << temperatureField[0+1][5+1] << "\n" << std::endl;       //+1 jeweils, weil die auf dem Arbeitsblatt bei den inneren werten erst mit 0 angefangen haben zu zählen
    std::cout << "delta_x: " << delta_x << "\ndelta_y: " << delta_y << "\n" << std::endl;
    std::cout << "Simulating...\n" <<std::endl;
    temperatureField = Calculations::calc_explicit_euler(timeStep, temperatureField, thermalDiffusivity, m, n, delta_x, delta_y);
    std::cout << "T_1,3:  " << temperatureField[1+1][3+1] << "\nT_0,3: " << temperatureField[0+1][3+1] << "\nT_0,5: " << temperatureField[0+1][5+1] << "\n" << std::endl;}

