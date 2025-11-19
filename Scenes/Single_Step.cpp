//
// Created by felix on 19.11.2025.
//

#include "Single_Step.h"

#include <glm/gtx/quaternion.hpp>

void Single_Step::init()
{
    //Calculate Inertia for half bounds
    float I_xx = M / 12 * (glm::pow(2 * b, 2) + glm::pow(2 * c, 2));
    float I_yy = M / 12 * (glm::pow(2 * a, 2) + glm::pow(2 * c, 2));
    float I_zz = M / 12 * (glm::pow(2 * a, 2) + glm::pow(2 * b, 2));

    I = glm::mat3{
        I_xx, 0.f, 0.f,
        0.f, I_yy, 0.f,
        0.f, 0.f, I_zz
    };

    //external forces:
    F = glm::vec3(1.f, 1.f, 0.f);
    q = glm::cross(glm::vec3(0.3f, 0.5f, 0.25f), F);

    //Euler Step
    v_cm = v_cm + step_size * F / M;
    x_cm = x_cm + step_size * v_cm;
    w = w + step_size * glm::inverse(I) * q;
    r = r + step_size * w;


    std::cout << "Updated Rigidbody State:" << std::endl;
    std::cout << "\nCenter of Mass Position: vec3(" << x_cm.x << ", " << x_cm.y << ", " << x_cm.z << ")" << std::endl;
    std::cout << "Center of Mass Linear Velocity: vec3(" << v_cm.x << ", " << v_cm.y << ", " << v_cm.z << ")" <<
        std::endl;
    std::cout << "Center of Mass Orientation: vec3(" << r.x << ", " << r.y << ", " << r.z << ")" << std::endl;
    std::cout << "Center of Mass Angular Velocity: vec3(" << w.x << ", " << w.y << ", " << w.z << ")" <<
        std::endl;


    //world position of point x_i
    float angle = glm::length(r);
    glm::vec3 axis = glm::normalize(r);
    glm::mat4 R4 = glm::rotate(glm::mat4(1.0f), angle, axis);
    glm::mat3 R = glm::mat3(R4);

    glm::vec3 x_rel = R * glm::vec3(-0.3f, -0.5f, -0.25f);
    glm::vec3 x_i = x_cm + x_rel;
    glm::vec3 v_i = v_cm + glm::cross(w, x_rel);

    std::cout << "\n\n\nUpdated Point State:" << std::endl;
    std::cout << "\nPoint Position: vec3(" << x_i.x << ", " << x_i.y << ", " << x_i.z << ")" << std::endl;
    std::cout << "Point Velocity: vec3(" << v_i.x << ", " << v_i.y << ", " << v_i.z << ")" <<
        std::endl;
}
