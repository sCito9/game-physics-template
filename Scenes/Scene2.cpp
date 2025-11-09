//
// Created by tonib on 09.11.2025.
//

#include "Scene2.h"
#include <imgui.h>

void Scene2::init() {
    MassPoint* point1 = new MassPoint(10, {0, 0, 0}, {-1, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point2 = new MassPoint(10, {0, 2, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0});
    massPoints.push_back(point1);
    massPoints.push_back(point2);

    Spring *s = new Spring(40, 1, point1, point2);
    springs.push_back(s);
}

void Scene2::onGUI() {
    ImGui::InputFloat("Timestep: ", &timeStep);
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        //calculateEulerStep();
    }

}

void Scene2::simulateStep() {
    calculateEulerStep();
}

