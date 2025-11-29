//
// Created by felix on 27.11.2025.
//

#include "Simulation.h"

#include "imgui.h"

void Simulation::onDraw(Renderer& renderer)
{
    for (auto vertex : cube.points_world_space)
    {
        renderer.drawSphere(vertex, 0.1f, glm::vec4(1, 0, 0, 1));
    }

    renderer.drawSphere(glm::vec3(force_position[0], force_position[1], force_position[2]), 0.05,
                        glm::vec4(0.f, 0.f, 1.f, 1.f));

    renderer.drawLine(cube.points_world_space[0], cube.points_world_space[1], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[1], cube.points_world_space[2], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[2], cube.points_world_space[4], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[4], cube.points_world_space[0], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[3], cube.points_world_space[5], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[5], cube.points_world_space[7], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[7], cube.points_world_space[6], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[6], cube.points_world_space[3], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[0], cube.points_world_space[7], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[1], cube.points_world_space[6], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[2], cube.points_world_space[3], glm::vec4(0, 1, 0, 1));
    renderer.drawLine(cube.points_world_space[4], cube.points_world_space[5], glm::vec4(0, 1, 0, 1));
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
        q = glm::vec3(glm::cross(glm::vec3(force_position[0], force_position[1], force_position[2]) - cube.position_cm,
                                 F));
    }
}

void Simulation::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;

    cur_delta_time += realtimeDt;
    for (; cur_delta_time >= step_size; cur_delta_time -= step_size)
    {
        simulateCube(&cube);
        resetForces();
    }
}


void Simulation::init()
{
    //rotate by 90 um z
    glm::vec3 axis(0.f, 0.f, 1.f);
    float angle = glm::radians(90.f);

    cube.orientation = glm::angleAxis(angle, glm::normalize(axis));

    //set inertia
    cube.I_inv = glm::mat3_cast(cube.orientation) * cube.I_inv * glm::transpose(glm::mat3_cast(cube.orientation));
    F = glm::vec3(1.f, 1.f, 0.f);
    q = glm::vec3(glm::cross(glm::vec3(0.3f, 0.5f, 0.25f) - cube.position_cm, F));
}

glm::quat Simulation::multiplication(glm::quat q1, glm::quat q2)
{
    float s1 = q1.w;
    float s2 = q2.w;
    glm::vec3 v1 = glm::vec3(q1.x, q1.y, q1.z);
    glm::vec3 v2 = glm::vec3(q2.x, q2.y, q2.z);

    return glm::quat(s1 * s2 - glm::dot(v1, v2), s1 * v2 + s2 * v1 + glm::cross(v1, v2));
}

void Simulation::resetForces()
{
    q = glm::vec3(0.f);
    F = glm::vec3(0.f);
}

void Simulation::simulateCube(Cube* obj)
{
    obj->velocity_cm += step_size * F / obj->M;
    obj->position_cm += step_size * obj->velocity_cm;

    obj->L += step_size * q;
    w = obj->I_inv * obj->L;
    glm::quat temp = step_size / 2.f * multiplication(obj->orientation, glm::quat(0.f, w.x, w.y, w.z));
    obj->orientation = glm::quat(obj->orientation.w + temp.w, obj->orientation.x + temp.x, obj->orientation.y + temp.y,
                                 obj->orientation.z + temp.z);
    obj->orientation = glm::normalize(obj->orientation);
    obj->I_inv = glm::mat3_cast(obj->orientation) * obj->I * glm::transpose(glm::mat3_cast(obj->orientation));

    simulatePointsWorldSpace(obj);
}

void Simulation::simulatePointsWorldSpace(Cube* obj)
{
    for (int i = 0; i < 8; ++i)
    {
        obj->points_world_space[i] = obj->position_cm + glm::mat3_cast(obj->orientation) * obj->points[i];
    }
}



