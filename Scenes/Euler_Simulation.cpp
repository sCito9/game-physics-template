//
// Created by felix on 31.10.2025.
//

#include "Euler_Simulation.h"

#include "imgui.h"

void Euler_Simulation::onDraw(Renderer& renderer)
{
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    renderer.drawLine(points[0].position, points[1].position, glm::vec4(0, 1, 0, 1));

    renderer.drawSphere(points[0].position, 0.1f, points[0].color);
    renderer.drawSphere(points[1].position, 0.1f, points[1].color);
}


void Euler_Simulation::onGUI()
{
    ImGui::SliderFloat("Time Step", &time_step, 0.001f, 0.5f);
}

void Euler_Simulation::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;

    //setup:
    double len, stretch;
    glm::dvec3 F01, F10;

    //Euler step
    glm::vec3 vel_0, vel_1, pos_0, pos_1;

    glm::dvec3 diff, dir;

    cur_delta_time += realtimeDt;
    for (; cur_delta_time >= time_step; cur_delta_time -= time_step)
    {
        //spring length
        diff = glm::dvec3(points[0].position) - glm::dvec3(points[1].position);
        len = glm::length(diff);

        dir = diff / len;
        stretch = len - static_cast<double>(spring.initial_Length);
        F01 = -static_cast<double>(spring.stiffness) * stretch * dir;
        F10 = -F01;

        //Euler step
        vel_0 = points[0].velocity + (glm::vec3(F01) / points[0].mass) * time_step;
        vel_1 = points[1].velocity + (glm::vec3(F10) / points[1].mass) * time_step;

        pos_0 = points[0].position + points[0].velocity * time_step;
        pos_1 = points[1].position + points[1].velocity * time_step;

        points[0].position = pos_0;
        points[0].velocity = vel_0;

        points[1].position = pos_1;
        points[1].velocity = vel_1;
    }
}

