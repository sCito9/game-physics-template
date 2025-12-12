//
// Created by felix on 27.11.2025.
//

#include "Simulation.h"

#include "imgui.h"

void Simulation::onDraw(Renderer& renderer)
{
    renderer.drawSphere(glm::vec3(force_position[0], force_position[1], force_position[2]), 0.05,
                        glm::vec4(0.f, 0.f, 1.f, 1.f));
    renderer.drawCube(cube.position_cm, cube.orientation, cube.scale, glm::vec4(1.f, 0.f, 0.f, 1.f));
}


void Simulation::onGUI()
{
    ImGui::SliderFloat("Time Step", &step_size, 0.001f, 4.f);
    //Leave y at 0 except you want the cube to be gone
    ImGui::InputFloat3("Force Amount", force_amount);
    ImGui::InputFloat3("Force Position", force_position);
    auto klicked = ImGui::Button("Add Force");

    if (klicked)
    {
        F = glm::vec3(force_amount[0], force_amount[1], force_amount[2]);
    }
}

void Simulation::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;

    cur_delta_time += realtimeDt;
    for (; cur_delta_time >= step_size; cur_delta_time -= step_size)
    {
        simulateCube(&cube, F, glm::vec3(force_position[0], force_position[1], force_position[2]), false,
                     step_size);
        F = glm::vec3(0.f);
    }
}

void Simulation::init()
{
    cube = initializeCube(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.6f, 0.5f), 2.f, glm::vec3(0.f, 0.f, 90.f),
                          glm::vec3(0.f, 0.f, 0.f),
                          glm::vec3(0.f));
}



