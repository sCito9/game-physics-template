//
// Created by tonib on 09.11.2025.
//

#include "Scene3.h"
#include <imgui.h>

void Scene3::init() {
    MassPoint* point1 = new MassPoint(10, {0, 0, 0}, {-1, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point2 = new MassPoint(10, {0, 2, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0});
    massPoints.push_back(point1);
    massPoints.push_back(point2);

    Spring *s = new Spring(40, 1, point1, point2);
    springs.push_back(s);
}

void Scene3::onGUI() {
    ImGui::InputFloat("Timestep: ", &timeStep);
}


void Scene3::simulateStep() {
    calculateMidpointStep();
}
