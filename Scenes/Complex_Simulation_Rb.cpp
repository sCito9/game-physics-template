//
// Created by felix on 29.11.2025.
//

#include "Complex_Simulation_Rb.h"

#include "imgui.h"

void Complex_Simulation_Rb::onDraw(Renderer& renderer)
{
    renderer.drawSphere(glm::vec3(force_position[0], force_position[1], force_position[2]), 0.05,
                        glm::vec4(0.f, 0.f, 1.f, 1.f));
    for (auto cube : cubes)
    {
        renderer.drawCube(cube->position_cm, cube->orientation, cube->scale, glm::vec4(1, 0, 0, 1));
    }
}

void Complex_Simulation_Rb::onGUI()
{
    ImGui::SliderFloat("Time Step", &step_size, 0.001f, 4.f);
    ImGui::SliderFloat("Restitution", &c, 0.f, 1.f);
    ImGui::Checkbox("Gravity", &gravity);
    ImGui::Text(
        "Add a force at the specified position (visualized by the blue ball) with the specified amount. Add via the button");
    ImGui::InputFloat3("Force Amount", force_amount);
    ImGui::InputFloat3("Force Position", force_position);
    auto klicked = ImGui::Button("Add Force");

    if (klicked)
    {
        F = glm::vec3(force_amount[0], force_amount[1], force_amount[2]);
    }

    ImGui::Text("Implosion shoots all the cubes towards the origin.");
    auto implosion = ImGui::Button("Implode");

    if (implosion)
    {
        for (auto& cube : cubes)
        {
            glm::vec3 direction = glm::normalize(-cube->position_cm);

            float distance = glm::length(cube->position_cm - glm::vec3(0.f));
            float minDistance = 0.f;
            float maxDistance = 40.f;
            float minScale = gravity ? 5.f : 0.f;
            float maxScale = gravity ? 35.f : 30.f;


            float scale = glm::clamp((distance - minDistance) / (maxDistance - minDistance), 0.f, 1.f);
            cube->velocity_cm = direction * glm::mix(minScale, maxScale, scale);
        }
    }
}

void Complex_Simulation_Rb::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;
    cur_delta_time += realtimeDt;
    for (; cur_delta_time >= step_size; cur_delta_time -= step_size)
    {
        checkCollisions(cubes, c);
        for (auto cube : cubes)
        {
            simulateCube(cube, F, glm::vec3(force_position[0], force_position[1], force_position[2]), gravity,
                         step_size);
        }
        F = glm::vec3(0.f);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        auto drag = ImGui::GetMouseDragDelta(1);
        if (!(drag.x == 0 && drag.y == 0))
        {
            auto dx = drag.x * right;
            auto dy = -drag.y * up;
            for (auto& cube : cubes)
            {
                cube->velocity_cm += (dx + dy) * 0.01f;
            }
        }
    }
}


void Complex_Simulation_Rb::init()
{
    cube_1 = initializeCube(glm::vec3(-4.f, 0.f, 0.f), glm::vec3(1.f), 2.f, glm::vec3(0.f), glm::vec3(4.f, 0.f, 0.f),
                            glm::vec3(0.f));

    cube_2 = initializeCube(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f), 4.f, glm::vec3(0.f, 45.f, 45.f),
                            glm::vec3(0.f, 0.f, 0.f),
                            glm::vec3(0.f));

    cube_3 = initializeCube(glm::vec3(5.f, 0.f, 0.f), glm::vec3(1.f), 3.f, glm::vec3(0.f), glm::vec3(-4.f, 0.f, 0.f),
                            glm::vec3(0.f));

    cube_4 = initializeCube(glm::vec3(0.f, 0.f, 5.f), glm::vec3(1.f), 6.f, glm::vec3(0.f, 45.f, 45.f),
                            glm::vec3(0.f, 0.f, -4.f),
                            glm::vec3(0.f));

    cube_5 = initializeCube(glm::vec3(0.f, 0.f, -8.f), glm::vec3(1.f), 2.f, glm::vec3(5.f, 0.f, 30.f),
                            glm::vec3(0.f, 0.f, 10.f),
                            glm::vec3(0.f));

    cubes = {&cube_1, &cube_2, &cube_3, &cube_4, &cube_5};
}


