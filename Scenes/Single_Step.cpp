//
// Created by felix on 19.11.2025.
//

#include "Single_Step.h"

#include <glm/gtx/quaternion.hpp>

void Single_Step::init()
{
    cube = initializeCube(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.6f, 0.5f), 2.f, glm::vec3(0.f, 0.f, 90.f),
                          glm::vec3(0.f, 0.f, 0.f),
                          glm::vec3(0.f));


    //External Forces
    glm::vec3 F = glm::vec3(1.f, 1.f, 0.f);

    simulateCube(&cube, F, glm::vec3(0.3f, 0.5f, 0.25f), false,
                 step_size);

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

    //As you have a mistake on the task description (x is defined differently two times) i will print it both ways
    //world position of point x_i
    glm::vec3 x_i = glm::vec3(-0.3f, -0.5f, -0.25f);
    x_i = cube.position_cm + glm::mat3_cast(cube.orientation) * x_i;

    glm::vec3 v_i = cube.velocity_cm + glm::cross(cube.w, glm::vec3(-0.3f, -0.5f, -0.25f));

    std::cout << "\n\n\nUpdated Point State:" << std::endl;
    std::cout << "\nPoint Position: vec3(" << x_i.x << ", " << x_i.y << ", " << x_i.z << ")" << std::endl;
    std::cout << "Point Velocity: vec3(" << v_i.x << ", " << v_i.y << ", " << v_i.z << ")" <<
        std::endl;
    std::cout << std::endl;


    glm::vec3 x_i_pos = glm::vec3(0.3f, 0.5f, 0.25f);
    x_i_pos = cube.position_cm + glm::mat3_cast(cube.orientation) * x_i_pos;

    glm::vec3 v_i_pos = cube.velocity_cm + glm::cross(cube.w, glm::vec3(0.3f, 0.5f, 0.25f));

    std::cout << "\n\n\nUpdated Point State positive values:" << std::endl;
    std::cout << "\nPoint Position: vec3(" << x_i_pos.x << ", " << x_i_pos.y << ", " << x_i_pos.z << ")" << std::endl;
    std::cout << "Point Velocity: vec3(" << v_i_pos.x << ", " << v_i_pos.y << ", " << v_i_pos.z << ")" <<
        std::endl;
}


