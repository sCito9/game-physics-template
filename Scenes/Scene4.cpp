//
// Created by tonib on 18.12.2025.
//

#include "Scene4.h"

#include "Calculations.h"
#include "imgui.h"


static const char* methods[] = { "Explicit Method", "Implicit Method" }; // Options for the dropdown
static int currentMethod = 1; // Index of the currently selected item

void Scene4::init() {
    domain = {{0, 24}, {0, 51}};
    m = 16;
    n = 16;
    temperatureField = Calculations::initializeTemperatureField(m, n);

    delta_x = Calculations::calc_delta_x(domain, n);
    delta_y = Calculations::calc_delta_y(domain, m);

    dimensionsMax = (currentMethod == 1) ? 200 : 500;
}

void Scene4::onGUI() {
    ImGui::SliderFloat("TimeStep", &timeStep, 0.001f, 1.0f);
    ImGui::SliderFloat("thermal Diffusivity", &thermalDiffusivity, 0.f, 1.f);
    if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
        shouldSimulate = !shouldSimulate;
    }
    ImGui::Checkbox("realDeltaTime", &realDeltaTime);
    ShowDropdownMenuForMethods();
    ImGui::Text((shouldSimulate) ? "Simulating..." : "Simulation stopped");

    if (ImGui::InputFloat("domain: x_from", &domain[0][0]) || ImGui::InputFloat("domain: x_to", &domain[0][1]) || ImGui::InputFloat("domain: y_from", &domain[1][0]) || ImGui::InputFloat("domain: y_to", &domain[1][1])) {
        delta_x = Calculations::calc_delta_x(domain, n);
        delta_y = Calculations::calc_delta_y(domain, m);
    }
    if (ImGui::SliderInt("m", &m, 0, dimensionsMax) || ImGui::SliderInt("n", &n, 0, dimensionsMax)) {
        //temperatureField = Calculations::coolerResizeTemperatureField(temperatureField, m, n, &coolerResizeVariety);
        temperatureField = Calculations::resizeTemperatureField(temperatureField, m, n);
        delta_x = Calculations::calc_delta_x(domain, n);
        delta_y = Calculations::calc_delta_y(domain, m);
    }
    ImGui::InputFloat("Temp to assing via click", &tempToAssignViaClick);
    if (ImGui::IsMouseDown(0)) {
        std::cout << "Clicked Pixel: " << static_cast<int>(ImGui::GetMousePos().x/windowWidth * (n+2)) << ", " << static_cast<int>(ImGui::GetMousePos().y/windowHeight * (m+2))<< std::endl;
        int y = static_cast<int>(ImGui::GetMousePos().y/windowHeight * (m+2));
        int x = static_cast<int>(ImGui::GetMousePos().x/windowWidth * (n+2));
        if (!(y < 0 || y > m+1 || x < 0 || x > n+1)) {
            temperatureField[y][x] = tempToAssignViaClick;
        }
    }
}

void Scene4::ShowDropdownMenuForMethods() {

    // Create the dropdown menu
    if (ImGui::BeginCombo("Choose a Method", methods[currentMethod])) {
        for (int i = 0; i < IM_ARRAYSIZE(methods); i++) {
            bool isSelected = (currentMethod == i); // Check if this item is selected
            if (ImGui::Selectable(methods[i], isSelected)) {
                currentMethod = i; // Update the selected item

                switch (currentMethod) {
                    case 0:
                        dimensionsMax = 500;
                        break;
                    case 1:
                        dimensionsMax = 200;
                        if (m>200 || n>200) {
                            if (m > 200) {
                                m = 200;
                            }
                            if (n > 200) {
                                n = 200;
                            }
                            temperatureField = Calculations::resizeTemperatureField(temperatureField, m, n);
                            delta_x = Calculations::calc_delta_x(domain, n);
                            delta_y = Calculations::calc_delta_y(domain, m);
                        }
                        break;
                    default:
                        break;
                }
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus(); // Focus the selected item
            }
        }
        ImGui::EndCombo(); // End the dropdown menu
    }
}

void Scene4::simulateStep() {
    if (!shouldSimulate) {
        return;
    }

    if (realDeltaTime) {
        float realTimeDt = ImGui::GetIO().DeltaTime;

        currentTime += realTimeDt;
        for (; currentTime >= timeStep; currentTime -= timeStep) {
            switch (currentMethod) {
                case 0:
                    temperatureField = Calculations::calc_explicit_euler(timeStep, temperatureField, thermalDiffusivity, m, n, delta_x, delta_y);
                    break;
                case 1:
                    temperatureField = Calculations::calc_implicit_euler(timeStep, temperatureField, thermalDiffusivity, m, n, delta_x, delta_y);
                    break;
                default:
                    break;
            }
        }
    }
    else {
        switch (currentMethod) {
            case 0:
                temperatureField = Calculations::calc_explicit_euler(timeStep, temperatureField, thermalDiffusivity, m, n, delta_x, delta_y);
                break;
            case 1:
                temperatureField = Calculations::calc_implicit_euler(timeStep, temperatureField, thermalDiffusivity, m, n, delta_x, delta_y);
                break;
            default:
                break;
        }
    }
}