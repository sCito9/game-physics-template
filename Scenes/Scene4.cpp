//
// Created by tonib on 10.12.2025.
//

#include "Scene4.h"

#include "imgui.h"

void Scene4::init() {
    cubes.emplace_back(new Rigidbody_Cube({0, 0, 0}, {12, 35, 1, 0}, {0, 0, 0}, 1.f, 1.f, 1.f, 2.f));
    cubes.emplace_back(new Rigidbody_Cube({2, 0, 0}, {2, 8, .38, 2.3}, {-.5f, 0, 0}, 1.f, 1.f, 1.f, 2.f));
    cubes.emplace_back(new Rigidbody_Cube({-2, 0, 0}, {123, 3.5, 45.8, 54}, {.5f, 0, 0}, 1.f, 1.f, 1.f, 2.f));
    cubes.emplace_back(new Rigidbody_Cube({0, 0, 2}, {0.21, 0.3, 0.26, 0.887}, {0, 0, -.5f}, 1.f, 1.f, 1.f, 2.f));

    //cubes.emplace_back(new Rigidbody_Cube({0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0}, 1.f, 1.f, 1.f, 2.f));
    cubes.emplace_back(new Rigidbody_Cube({0, 0, -3.f}, {0, 0, 0, 0}, {0, 0, 0}, 1.f, 1.f, 1.f, 99999999.f));
}

void Scene4::onGUI() {
    ImGui::Text("realDeltaTime: actually calls the update function every timestep frames. \n"
                "   -> with a timestep of 2 it will only call it every 2 seconds, which will look laggy. \n"
                "   Disable to simulate a custom passing of time, where the update function gets called every frame\n"
                "TimeStep: defines the timestep\n"
                "CollisionElasticity: defines the collision elasticity\n"
                "Space: Stop and Resume the Simulation\n"
                "Gravity: Toggles Gravity on and off\n"
                "Implode: Pulls all boxes towards (0, 0, 0)\n");
    ImGui::SliderFloat("TimeStep", &timeStep, 0.001f, 2.0f);
    ImGui::SliderFloat("CollisionElasticity", &collisionElasticity, 0.f, 1.f);
    if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
        shouldSimulate = !shouldSimulate;
    }
    ImGui::Checkbox("realDeltaTime", &realDeltaTime);
    ImGui::Checkbox("Gravity", &activeGravity);
    if (ImGui::Button("Implode")) {
        addImplosionForces();
    }
}

void Scene4::simulateStep() {
    if (!shouldSimulate) {
        return;
    }

    if (activeGravity) {
        for (auto& cube : cubes) {
            addForce(cube, glm::vec3(0, 0, 0), glm::vec3(0, 0, -9.81f));
        }
    }

    if (realDeltaTime) {
        float realTimeDt = ImGui::GetIO().DeltaTime;

        currentTime += realTimeDt;
        for (; currentTime >= timeStep; currentTime -= timeStep) {
            simStep(timeStep);
        }
    }
    else {
        simStep(timeStep);
    }
}

void Scene4::addImplosionForces() {
    for (auto& cube : cubes) {
        addForce(cube, glm::vec3(0, 0, 0), glm::normalize(-cube->cm_pos) * 500.f);
    }
}