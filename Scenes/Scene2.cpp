//
// Created by tonib on 29.11.2025.
//

#include "Scene2.h"

#include "imgui.h"

void Scene2::init() {
    glm::mat3 initialRotation = glm::mat3(cos(glm::pi<float>() / 2), -sin(glm::pi<float>() / 2), 0,
                                        sin(glm::pi<float>() / 2), cos(glm::pi<float>() /2), 0,
                                        0, 0, 1);
    cubes.emplace_back(new Rigidbody_Cube({0, 0, 0}, glm::toQuat(initialRotation), {0, 0, 0}, 1.f, 0.6f, 0.5f, 2));
}

void Scene2::onGUI() {
    ImGui::SliderFloat("TimeStep", &timeStep, 0.001f, 2.0f);
    if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
        shouldSimulate = !shouldSimulate;
    }
}


void Scene2::simulateStep() {
    if (!shouldSimulate) {
        return;
    }
    float realTimeDt = ImGui::GetIO().DeltaTime;

    currentTime += realTimeDt;
    for (; currentTime >= timeStep; currentTime -= timeStep) {
        simStep(timeStep);
    }
}

