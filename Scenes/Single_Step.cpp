//
// Created by felix on 31.10.2025.
//

#include "Single_Step.h"

#include <glm/vec3.hpp>

void Single_Step::init()
{
    std::cout << "Masspoint 0" << std::endl;
    std::cout << "position: vec3(" << point_0.position.x << ", " << point_0.position.y << ", " << point_0.position.z <<
        ")" << std::endl;
    std::cout << "velocity: vec3(" << point_0.velocity.x << ", " << point_0.velocity.y << ", " << point_0.velocity.z <<
        ")" << std::endl;

    std::cout << "Masspoint 1" << std::endl;
    std::cout << "position: vec3(" << point_1.position.x << ", " << point_1.position.y << ", " << point_1.position.z <<
        ")" << std::endl;
    std::cout << "velocity: vec3(" << point_1.velocity.x << ", " << point_1.velocity.y << ", " << point_1.velocity.z <<
        ")" << std::endl;

    //spring length
    glm::dvec3 diff = glm::dvec3(points[0].position) - glm::dvec3(points[1].position);
    double len = glm::length(diff);

    glm::dvec3 dir = diff / len;
    double stretch = len - static_cast<double>(spring.initial_Length);
    glm::dvec3 F01 = -static_cast<double>(spring.stiffness) * stretch * dir;
    glm::dvec3 F10 = -F01;


    //Euler step


    glm::vec3 vel_0 = point_0.velocity + (glm::vec3(F01) / point_0.mass) * 0.1f;
    glm::vec3 vel_1 = point_1.velocity + (glm::vec3(F10) / point_1.mass) * 0.1f;

    glm::vec3 pos_0 = point_0.position + point_0.velocity * 0.1f;
    glm::vec3 pos_1 = point_1.position + point_1.velocity * 0.1f;

    std::cout << "\n\n\nAfter 0.1s using Euler Method:" << std::endl;
    std::cout << "Masspoint 0" << std::endl;
    std::cout << "position: vec3(" << pos_0.x << ", " << pos_0.y << ", " << pos_0.z << ")" << std::endl;
    std::cout << "velocity: vec3(" << vel_0.x << ", " << vel_0.y << ", " << vel_0.z << ")" << std::endl;

    std::cout << "Masspoint 1" << std::endl;
    std::cout << "position: vec3(" << pos_1.x << ", " << pos_1.y << ", " << pos_1.z << ")" << std::endl;
    std::cout << "velocity: vec3(" << vel_1.x << ", " << vel_1.y << ", " << vel_1.z << ")" << std::endl;

    //Midpoint step
    glm::vec3 vel_0_m = point_0.velocity + (glm::vec3(F01) / point_0.mass) * (0.1f / 2.f);
    glm::vec3 vel_1_m = point_1.velocity + (glm::vec3(F10) / point_1.mass) * (0.1f / 2.f);

    glm::vec3 pos_0_m = point_0.position + point_0.velocity * (0.1f / 2.f);
    glm::vec3 pos_1_m = point_1.position + point_1.velocity * (0.1f / 2.f);


    //spring length
    diff = glm::dvec3(pos_0_m) - glm::dvec3(pos_1_m);
    len = glm::length(diff);

    dir = diff / len;
    stretch = len - static_cast<double>(spring.initial_Length);
    F01 = -static_cast<double>(spring.stiffness) * stretch * dir;
    F10 = -F01;


    vel_0 = point_0.velocity + (glm::vec3(F01) / point_0.mass) * 0.1f;
    vel_1 = point_1.velocity + (glm::vec3(F10) / point_1.mass) * 0.1f;

    pos_0 = point_0.position + vel_0_m * 0.1f;
    pos_1 = point_1.position + vel_1_m * 0.1f;


    std::cout << "\n\n\nAfter 0.1s using Midpoint Method:" << std::endl;
    std::cout << "Masspoint 0" << std::endl;
    std::cout << "position: vec3(" << pos_0.x << ", " << pos_0.y << ", " << pos_0.z << ")" << std::endl;
    std::cout << "velocity: vec3(" << vel_0.x << ", " << vel_0.y << ", " << vel_0.z << ")" << std::endl;

    std::cout << "Masspoint 1" << std::endl;
    std::cout << "position: vec3(" << pos_1.x << ", " << pos_1.y << ", " << pos_1.z << ")" << std::endl;
    std::cout << "velocity: vec3(" << vel_1.x << ", " << vel_1.y << ", " << vel_1.z << ")" << std::endl;
}
