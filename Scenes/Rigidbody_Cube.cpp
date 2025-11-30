//
// Created by tonib on 28.11.2025.
//

#include "Rigidbody_Cube.h"

Rigidbody_Cube::Rigidbody_Cube(glm::vec3 position, glm::quat rotation, glm::vec3 linearVelocity , float x_length, float y_length, float z_length, float mass){
    scale = glm::vec3(x_length, y_length, z_length);
    cm_pos = position;
    r = glm::normalize(rotation);
    cm_linearVelocity = linearVelocity;
    L = {0, 0, 0};
    rot = glm::toMat3(r);
    M = mass;

    I0_1 = glm::mat3(1.f / 12.f * mass * (y_length * y_length + z_length * z_length), 0, 0,
                     0, 1.f / 12.f * mass * (x_length * x_length + z_length * z_length), 0,
                     0, 0, 1.f / 12.f * mass * (x_length * x_length + y_length * y_length));
    I0_1 = glm::inverse(I0_1);
    I_1 = rot * I0_1 * glm::transpose(rot);
    angularVelocity = I_1 * L;

    //berechnet die lokalen offsets zu den punkten
    local_vertices_pos[0] = glm::vec3(-x_length / 2, -y_length / 2, -z_length / 2);
    local_vertices_pos[1] = glm::vec3(-x_length / 2, y_length / 2, -z_length / 2);
    local_vertices_pos[2] = glm::vec3(x_length / 2, -y_length / 2, -z_length / 2);
    local_vertices_pos[3] = glm::vec3(x_length / 2, y_length / 2, -z_length / 2);
    local_vertices_pos[4] = glm::vec3(-x_length / 2, -y_length / 2, z_length / 2);
    local_vertices_pos[5] = glm::vec3(-x_length / 2, y_length / 2, z_length / 2);
    local_vertices_pos[6] = glm::vec3(x_length / 2, -y_length / 2, z_length / 2);
    local_vertices_pos[7] = glm::vec3(x_length / 2, y_length / 2, z_length / 2);

    for (int i = 0; i < 8; i++) {
        //gibt den Punkten deren Worldpositions
        points[i] = Punkt(cm_pos + rot * local_vertices_pos[i], glm::vec3(0), M / 8);
    }
}
