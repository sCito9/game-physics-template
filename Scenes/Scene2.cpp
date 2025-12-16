//
// Created by tonib on 14.12.2025.
//

#include "Scene2.h"

#include "Calculations.h"
#include "imgui.h"

void Scene2::init() {
    domain = {{0, 1}, {0, 1}};
    m = 16;
    n = 16;
    temperatureField = Calculations::initializeTemperatureField(m, n);

    delta_x = Calculations::calc_delta_x(domain, n);
    delta_y = Calculations::calc_delta_y(domain, m);
}

void Scene2::onGUI() {
    ImGui::SliderFloat("TimeStep", &timeStep, 0.001f, 1.0f);
    ImGui::SliderFloat("thermal Diffusivity", &thermalDiffusivity, 0.f, 1.f);
    if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
        shouldSimulate = !shouldSimulate;
    }
    ImGui::Checkbox("realDeltaTime", &realDeltaTime);
}


void Scene2::simulateStep() {
    if (!shouldSimulate) {
        return;
    }

    if (realDeltaTime) {
        float realTimeDt = ImGui::GetIO().DeltaTime;

        currentTime += realTimeDt;
        for (; currentTime >= timeStep; currentTime -= timeStep) {
            temperatureField = Calculations::calc_explicit_euler(timeStep, temperatureField, thermalDiffusivity, m, n, delta_x, delta_y);
        }
    }
    else {
        temperatureField = Calculations::calc_explicit_euler(timeStep, temperatureField, thermalDiffusivity, m, n, delta_x, delta_y);
    }
}
