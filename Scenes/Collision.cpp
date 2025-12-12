//
// Created by felix on 28.11.2025.
//

#include "Collision.h"

#include "imgui.h"

void Collision::onDraw(Renderer& renderer)
{
    renderer.drawCube(cube_1.position_cm, cube_1.orientation, cube_1.scale, glm::vec4(1.f, 0.f, 0.f, 1.f));
    renderer.drawCube(cube_2.position_cm, cube_2.orientation, cube_2.scale, glm::vec4(0.f, 0.f, 1.f, 1.f));
}

void Collision::onGUI()
{
    ImGui::SliderFloat("Time Step", &step_size, 0.001f, 4.f);
    ImGui::SliderFloat("Restitution", &c, 0.f, 1.f);
}

void Collision::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;
    cur_delta_time += realtimeDt;
    for (; cur_delta_time >= step_size; cur_delta_time -= step_size)
    {
        checkCollisions(cubes, c);
        for (auto cube : cubes)
        {
            simulateCube(cube, F, glm::vec3(0.f), false,
                         step_size);
        }
        F = glm::vec3(0.f);
    }
}

void Collision::init()
{
    cube_1 = initializeCube(glm::vec3(-6.f, 0.f, 0.f), glm::vec3(1.f), 3.f, glm::vec3(0.f), glm::vec3(3.f, 0.f, 0.f),
                            glm::vec3(0.f));

    cube_2 = initializeCube(glm::vec3(3.f, 0.f, 0.f), glm::vec3(1.f), 2.f, glm::vec3(0.f, 45.f, 45.f),
                            glm::vec3(0.f, 0.f, 0.f),
                            glm::vec3(0.f));
    cubes = {&cube_1, &cube_2};
}
