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
    x_cm = x_cm + step_size * v_cm;
    v_cm = v_cm + step_size * F / M;
    r = r + step_size * w;
    w = w + step_size * q / I;


    //world position of point x_i
}
