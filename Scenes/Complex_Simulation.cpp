//
// Created by felix on 31.10.2025.
//

#include "Complex_Simulation.h"

#include "imgui.h"

void Complex_Simulation::onDraw(Renderer& renderer)
{
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    for (auto& point : points)
    {
        renderer.drawSphere(point.position, 0.05f, point.color);
    }

    for (auto& spring : springs)
    {
        renderer.drawLine(points[spring.point_1].position, points[spring.point_2].position,
                          glm::vec4(0, 1, 0, 1));
    }
}


void Complex_Simulation::onGUI()
{
    ImGui::SliderFloat("Time Step", &time_step, 0.001f, 0.5f);
    ImGui::SliderFloat("Bounce Factor", &bounce_Factor, -2.f, -0.1f);
    ImGui::SliderFloat("Damping Factor", &damping, 0.f, 1.f);

    const char* items[] = {"Euler Simulation", "Midpoint Simulation"};
    int current_index = static_cast<int>(currentState);

    if (ImGui::Combo("Method", &current_index, items, 2))
    {
        currentState = static_cast<State>(current_index);
    }

    ImGui::Checkbox("Fix Point 0:", &points[0].isFixed);
    ImGui::Checkbox("Fix Point 1:", &points[1].isFixed);
    ImGui::Checkbox("Fix Point 2:", &points[2].isFixed);
    ImGui::Checkbox("Fix Point 3:", &points[3].isFixed);
    ImGui::Checkbox("Fix Point 4:", &points[4].isFixed);
    ImGui::Checkbox("Fix Point 5:", &points[5].isFixed);
    ImGui::Checkbox("Fix Point 6:", &points[6].isFixed);
    ImGui::Checkbox("Fix Point 7:", &points[7].isFixed);
    ImGui::Checkbox("Fix Point 8:", &points[8].isFixed);
    ImGui::Checkbox("Fix Point 9:", &points[9].isFixed);
}

void Complex_Simulation::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;

    //setup:
    double len;
    glm::dvec3 F01, F10;


    glm::dvec3 diff;

    cur_delta_time += realtimeDt;
    for (; cur_delta_time >= time_step; cur_delta_time -= time_step)
    {
        if (currentState == State::Euler_Simulation)
        {
            eulerMethod(len, diff, F01, F10);
        }
        else
        {
            midpointMethod(len, diff, F01, F10);
        }

        //Collision
        checkCollision();
    }
}

void Complex_Simulation::eulerMethod(double len, glm::dvec3 diff, glm::dvec3 F01, glm::dvec3 F10)
{
    double stretch;
    glm::dvec3 dir;
    glm::vec3 forces[std::size(points)] = {};
    //compute forces
    for (auto& spring : springs)
    {
        //spring length
        diff = glm::dvec3(points[spring.point_1].position) - glm::dvec3(points[spring.point_2].position);
        len = glm::length(diff);

        dir = diff / len;
        stretch = len - static_cast<double>(spring.initial_Length);
        F01 = -static_cast<double>(spring.stiffness) * stretch * dir;
        F10 = -F01;

        forces[spring.point_1] += glm::vec3(F01);
        forces[spring.point_2] += glm::vec3(F10);
    }

    //apply gravity
    for (int j = 0; j < std::size(points); j++)
    {
        forces[j] += points[j].mass * gravity;
    }

    //calculate euler step
    for (int j = 0; j < std::size(points); j++)
    {
        if (points[j].isFixed) continue;
        points[j].position = points[j].position + points[j].velocity * time_step;
        points[j].velocity = points[j].velocity + (forces[j] / points[j].mass) * time_step * damping;
    }
}


void Complex_Simulation::midpointMethod(double len, glm::dvec3 diff, glm::dvec3 F01, glm::dvec3 F10)
{
    double stretch;
    glm::dvec3 dir;
    glm::vec3 forces[std::size(points)] = {};

    //reset forces
    for (auto& force : forces)
    {
        force = glm::vec3(0.f);
    }

    //compute forces
    for (auto& spring : springs)
    {
        //spring length
        diff = glm::dvec3(points[spring.point_1].position) - glm::dvec3(points[spring.point_2].position);
        len = glm::length(diff);

        dir = diff / len;
        stretch = len - static_cast<double>(spring.initial_Length);
        F01 = -static_cast<double>(spring.stiffness) * stretch * dir;
        F10 = -F01;

        forces[spring.point_1] += glm::vec3(F01);
        forces[spring.point_2] += glm::vec3(F10);
    }

    //add gravity
    for (int j = 0; j < std::size(points); j++)
    {
        forces[j] += points[j].mass * gravity;
    }

    //Midpoint step

    glm::vec3 vel_m[std::size(points)] = {};
    glm::vec3 pos_m[std::size(points)] = {};

    //apply mid values
    for (int j = 0; j < std::size(points); j++)
    {
        if (points[j].isFixed) continue;
        vel_m[j] = points[j].velocity + (forces[j] / points[j].mass) * (time_step / 2.f) * damping;
        pos_m[j] = points[j].position + points[j].velocity * (time_step / 2.f);
    }


    //full step

    //reset forces
    for (auto& force : forces)
    {
        force = glm::vec3(0.f);
    }

    //compute forces
    for (auto& spring : springs)
    {
        //spring length
        diff = glm::dvec3(pos_m[spring.point_1]) - glm::dvec3(pos_m[spring.point_2]);
        len = glm::length(diff);

        dir = diff / len;
        stretch = len - static_cast<double>(spring.initial_Length);
        F01 = -static_cast<double>(spring.stiffness) * stretch * dir;
        F10 = -F01;

        forces[spring.point_1] += glm::vec3(F01);
        forces[spring.point_2] += glm::vec3(F10);
    }

    //apply gravity
    for (int j = 0; j < std::size(points); j++)
    {
        forces[j] += points[j].mass * gravity;
    }

    //apply changes
    for (int j = 0; j < std::size(points); j++)
    {
        if (points[j].isFixed) continue;
        points[j].velocity = points[j].velocity + (forces[j] / points[j].mass) * time_step * damping;
        points[j].position = points[j].position + vel_m[j] * time_step;
    }
}


void Complex_Simulation::checkCollision()
{
    for (int j = 0; j < std::size(points); j++)
    {
        if (points[j].isFixed) continue;

        if (points[j].position.x < cube_min.x)
        {
            points[j].position.x = cube_min.x;
            points[j].velocity.x *= bounce_Factor;
        }
        if (points[j].position.x > cube_max.x)
        {
            points[j].position.x = cube_max.x;
            points[j].velocity.x *= bounce_Factor;
        }

        if (points[j].position.y < cube_min.y)
        {
            points[j].position.y = cube_min.y;
            points[j].velocity.y *= bounce_Factor;
        }
        if (points[j].position.y > cube_max.y)
        {
            points[j].position.y = cube_max.y;
            points[j].velocity.y *= bounce_Factor;
        }

        if (points[j].position.z < cube_min.z)
        {
            points[j].position.z = cube_min.z;
            points[j].velocity.z *= bounce_Factor;
        }
        if (points[j].position.z > cube_max.z)
        {
            points[j].position.z = cube_max.z;
            points[j].velocity.z *= bounce_Factor;
        }
    }
}
