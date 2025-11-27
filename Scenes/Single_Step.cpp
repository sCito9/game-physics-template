//
// Created by felix on 19.11.2025.
//

#include "Single_Step.h"

#include <glm/gtx/quaternion.hpp>

void Single_Step::init()
{
    //rotate by 90 um z
    glm::vec3 axis(0.f, 0.f, 1.f);
    float angle = glm::radians(90.f);

    cube.orientation = glm::angleAxis(angle, glm::normalize(axis));

    //set inertia
    cube.I_inv = glm::mat3_cast(cube.orientation) * cube.I_inv * glm::transpose(glm::mat3_cast(cube.orientation));

    //initialize w
    glm::vec3 w = glm::vec3(0.f);


    //External Forces
    glm::vec3 F = glm::vec3(1.f, 1.f, 0.f);
    glm::vec3 q = glm::vec3(glm::cross(glm::vec3(0.3f, 0.5f, 0.25f) - cube.position_cm, F));

    //compute
    cube.velocity_cm += step_size * F / cube.M;
    cube.position_cm += step_size * cube.velocity_cm;

    cube.L += step_size * q;
    w = cube.I_inv * cube.L;
    cube.orientation += step_size / 2.f * multiplication(glm::quat(0.f, w.x, w.y, w.z), cube.orientation);
    cube.orientation = glm::normalize(cube.orientation);
    cube.I_inv = glm::mat3_cast(cube.orientation) * cube.I_inv * glm::transpose(glm::mat3_cast(cube.orientation));

    std::cout << "Updated Rigidbody State:" << std::endl;
    std::cout << "\nCenter of Mass Position: vec3(" << cube.position_cm.x << ", " << cube.position_cm.y << ", " << cube.
        position_cm.z << ")" << std::endl;
    std::cout << "Center of Mass Linear Velocity: vec3(" << cube.velocity_cm.x << ", " << cube.velocity_cm.y << ", " <<
        cube.velocity_cm.z << ")" <<
        std::endl;
    std::cout << "Center of Mass Orientation: " << "(" << cube.orientation.w << ", vec3(" << cube.orientation.x << ", "
        << cube.orientation.y << ", " << cube.orientation.z << "))" << std::endl;
    std::cout << "Center of Mass Angular Velocity: vec3(" << cube.L.x << ", " << cube.L.y << ", " << cube.L.z << ")" <<
        std::endl;


    //world position of point x_i
    glm::vec3 x_i = glm::vec3(-0.3f, -0.5f, -0.25f);
    glm::vec3 v_i = glm::vec3(0.f);

    x_i = cube.position_cm + glm::mat3_cast(cube.orientation) * x_i;
    v_i = cube.velocity_cm + glm::cross(w, x_i);

    std::cout << "\n\n\nUpdated Point State:" << std::endl;
    std::cout << "\nPoint Position: vec3(" << x_i.x << ", " << x_i.y << ", " << x_i.z << ")" << std::endl;
    std::cout << "Point Velocity: vec3(" << v_i.x << ", " << v_i.y << ", " << v_i.z << ")" <<
        std::endl;
}

glm::quat Single_Step::multiplication(glm::quat q1, glm::quat q2)
{
    float s1 = q1.w;
    float s2 = q2.w;
    glm::vec3 v1 = glm::vec3(q1.x, q1.y, q1.z);
    glm::vec3 v2 = glm::vec3(q2.x, q2.y, q2.z);

    return glm::quat(s1 * s2 - glm::dot(v1, v2), s1 * v2 + s2 * v1 + glm::cross(v1, v2));
}

