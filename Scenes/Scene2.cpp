//
// Created by tonib on 29.11.2025.
//

#include "Scene2.h"
#include "Scene.h"

#include "imgui.h"

void Scene2::init() {
    glm::mat3 initialRotation = glm::mat3(cos(glm::pi<float>() / 2), -sin(glm::pi<float>() / 2), 0,
                                        sin(glm::pi<float>() / 2), cos(glm::pi<float>() /2), 0,
                                        0, 0, 1);
    cubes.emplace_back(new Rigidbody_Cube({0, 0, 0}, glm::toQuat(initialRotation), {0, 0, 0}, 1.f, 0.6f, 0.5f, 2.f));
    cubes.front()->ForcePosQueue.push_back(glm::vec3(0.3f, 0.5f, 0.25f));
    cubes.front()->ForceDirQueue.push_back(glm::vec3(1, 1, 0));
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

    handleInput(cubes.front());

    float realTimeDt = ImGui::GetIO().DeltaTime;

    currentTime += realTimeDt;
    for (; currentTime >= timeStep; currentTime -= timeStep) {
        simStep(timeStep);
    }
}

float stärke = 1;
void Scene2::handleInput(Rigidbody_Cube* cube) {
    if (ImGui::IsKeyPressed(ImGuiKey_W)) {
        cube->ForcePosQueue.emplace_back(glm::vec3(0, 0, 0));
        cube->ForceDirQueue.emplace_back(glm::normalize(up) * stärke);
    }
    if (ImGui::IsKeyPressed(ImGuiKey_S)) {
        cube->ForcePosQueue.emplace_back(glm::vec3(0, 0, 0));
        cube->ForceDirQueue.emplace_back(glm::normalize(up) * -stärke);
    }
    if (ImGui::IsKeyPressed(ImGuiKey_A)) {
        cube->ForcePosQueue.emplace_back(glm::vec3(0, 0, 0));
        cube->ForceDirQueue.emplace_back(glm::normalize(right) * -stärke);
    }
    if (ImGui::IsKeyPressed(ImGuiKey_D)) {
        cube->ForcePosQueue.emplace_back(glm::vec3(0, 0, 0));
        cube->ForceDirQueue.emplace_back(glm::normalize(right) * stärke);
    }
    if (ImGui::IsKeyPressed(ImGuiKey_E)) {
        cube->ForcePosQueue.emplace_back(glm::normalize(up) * 0.1f);
        cube->ForceDirQueue.emplace_back(glm::normalize(right) * stärke);

        cube->ForcePosQueue.emplace_back(glm::normalize(up) * -0.1f);
        cube->ForceDirQueue.emplace_back(glm::normalize(right) * -stärke);
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
        cube->ForcePosQueue.emplace_back(glm::normalize(up) * 0.1f);
        cube->ForceDirQueue.emplace_back(glm::normalize(right) * -stärke);

        cube->ForcePosQueue.emplace_back(glm::normalize(up) * -0.1f);
        cube->ForceDirQueue.emplace_back(glm::normalize(right) * stärke);
    }
}

