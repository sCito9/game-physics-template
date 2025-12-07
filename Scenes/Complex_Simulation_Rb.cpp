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
        /* for (auto vertex : cube->points_world_space)
         {
             renderer.drawSphere(vertex, 0.1f, glm::vec4(1, 0, 0, 1));
         }

         renderer.drawCube(cube->position_cm, cube->orientation, cube->scale, glm::vec4(1, 0, 0, 1));

         renderer.drawLine(cube->points_world_space[0], cube->points_world_space[1], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[0], cube->points_world_space[3], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[0], cube->points_world_space[4], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[1], cube->points_world_space[2], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[1], cube->points_world_space[5], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[2], cube->points_world_space[3], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[2], cube->points_world_space[6], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[3], cube->points_world_space[7], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[4], cube->points_world_space[5], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[4], cube->points_world_space[7], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[5], cube->points_world_space[6], glm::vec4(0, 1, 0, 1));
         renderer.drawLine(cube->points_world_space[6], cube->points_world_space[7], glm::vec4(0, 1, 0, 1));*/
    }
}


void Complex_Simulation_Rb::onGUI()
{
    ImGui::SliderFloat("Time Step", &step_size, 0.001f, 4.f);
    ImGui::SliderFloat("Restitution", &c, 0.f, 1.f);
    ImGui::Checkbox("Gravity", &gravity);
    ImGui::InputFloat3("Force Amount", force_amount);
    ImGui::InputFloat3("Force Position", force_position);
    auto klicked = ImGui::Button("Add Force");

    if (klicked)
    {
        F = glm::vec3(force_amount[0], force_amount[1], force_amount[2]);
    }
}

void Complex_Simulation_Rb::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;
    cur_delta_time += realtimeDt;
    for (; cur_delta_time >= step_size; cur_delta_time -= step_size)
    {
        checkCollisions(cubes);
        for (auto cube : cubes)
        {
            simulateCube(cube);
        }
        resetForces();
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
                if (cube->isFixed) continue;
                cube->velocity_cm += (dx + dy) * 0.01f;
            }
        }
    }
}


void Complex_Simulation_Rb::init()
{
    //rotate cube_2
    glm::vec3 axis(0.f, 1.f, 1.f);
    float angle = glm::radians(45.f);

    cube_2.orientation = glm::angleAxis(angle, glm::normalize(axis));


    //recompute for rotated cube_2
    cube_2.I_inv = glm::mat3_cast(cube_2.orientation) * cube_2.I_inv * glm::transpose(
        glm::mat3_cast(cube_2.orientation));
}

glm::quat Complex_Simulation_Rb::multiplication(glm::quat q1, glm::quat q2)
{
    float s1 = q1.w;
    float s2 = q2.w;
    glm::vec3 v1 = glm::vec3(q1.x, q1.y, q1.z);
    glm::vec3 v2 = glm::vec3(q2.x, q2.y, q2.z);

    return glm::quat(s1 * s2 - glm::dot(v1, v2), s1 * v2 + s2 * v1 + glm::cross(v1, v2));
}

void Complex_Simulation_Rb::resetForces()
{
    q = glm::vec3(0.f);
    F = glm::vec3(0.f);
}

void Complex_Simulation_Rb::simulateCube(Cube* obj)
{
    //add gravity


    glm::vec3 F_gravity = F;
    if (gravity && !obj->isFixed) F_gravity += obj->M * glm::vec3(0.f, 0.f, -9.81f);

    q = glm::vec3(glm::cross(glm::vec3(force_position[0], force_position[1], force_position[2]) - obj->position_cm,
                             F));
    obj->velocity_cm += step_size * F_gravity / obj->M;
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

void Complex_Simulation_Rb::simulatePointsWorldSpace(Cube* obj)
{
    for (int i = 0; i < 8; ++i)
    {
        obj->points_world_space[i] = obj->position_cm + glm::mat3_cast(obj->orientation) * obj->points[i];
    }
}

glm::mat4 Complex_Simulation_Rb::getWorldFromObj(Cube* cube)
{
    glm::mat4 rotationMatrix = glm::toMat4(cube->orientation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), cube->scale);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), cube->position_cm);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

void Complex_Simulation_Rb::checkCollisions(Cube* cube_array[])
{
    for (int i = 0; i < array_length - 1; ++i)
    {
        for (int j = i + 1; j < array_length; ++j)
        {
            glm::mat4 world_1 = getWorldFromObj(cube_array[i]);
            glm::mat4 world_2 = getWorldFromObj(cube_array[j]);
            CollisionInfo hitInfo = collisionTools::checkCollisionSAT(world_1, world_2);
            if (hitInfo.isColliding)
            {
                calculateCollision(hitInfo, cube_array[i], cube_array[j]);
            }
        }
    }
}

void Complex_Simulation_Rb::calculateCollision(CollisionInfo hitInfo, Cube* cube_1, Cube* cube_2)
{
    hitInfo.normalWorld = glm::normalize(hitInfo.normalWorld);
    glm::vec3 w_a = cube_1->I_inv * cube_1->L;
    glm::vec3 w_b = cube_2->I_inv * cube_2->L;
    //flip sign of normal if necessary
    glm::vec3 pre_collision_relative_velocity = cube_1->velocity_cm +
        glm::cross(w_a, hitInfo.collisionPointWorld - cube_1->position_cm) - cube_2->velocity_cm - glm::cross(
            w_b, hitInfo.collisionPointWorld - cube_2->position_cm);

    float rel_normal_dot = glm::dot(pre_collision_relative_velocity, hitInfo.normalWorld);
    if (rel_normal_dot > 0.f) return;

    glm::vec3 fromAtoB = cube_2->position_cm - cube_1->position_cm;
    if (glm::dot(hitInfo.normalWorld, fromAtoB) < 0.f) hitInfo.normalWorld = -hitInfo.normalWorld;


    float J = (-(1 + c) * glm::dot(pre_collision_relative_velocity, hitInfo.normalWorld)) /
    ((1 / cube_1->M) + (1 / cube_2->M) + glm::dot(glm::cross(cube_1->I_inv * (glm::cross(
                                                                 hitInfo.collisionPointWorld - cube_1->position_cm,
                                                                 hitInfo.normalWorld)),
                                                             hitInfo.collisionPointWorld - cube_1->position_cm),
                                                  hitInfo.normalWorld)
        + glm::dot(glm::cross(cube_2->I_inv * (glm::cross(hitInfo.collisionPointWorld - cube_2->position_cm,
                                                          hitInfo.normalWorld)),
                              hitInfo.collisionPointWorld - cube_2->position_cm), hitInfo.normalWorld));


    cube_1->velocity_cm = cube_1->velocity_cm + J * hitInfo.normalWorld / cube_1->M;
    cube_1->L = cube_1->L + glm::cross(hitInfo.collisionPointWorld - cube_1->position_cm, J * hitInfo.normalWorld);


    cube_2->velocity_cm = cube_2->velocity_cm - J * hitInfo.normalWorld / cube_2->M;
    cube_2->L = cube_2->L - glm::cross(hitInfo.collisionPointWorld - cube_2->position_cm, J * hitInfo.normalWorld);
}
