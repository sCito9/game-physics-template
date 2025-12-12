//
// Created by felix on 12.12.2025.
//

#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include <glm/fwd.hpp>
#include "Scene.h"


struct Cube;
struct CollisionInfo;


void simulateCube(Cube* obj, glm::vec3 F, glm::vec3 force_position, bool gravity, float step_size);
glm::mat4 getWorldFromObj(Cube* cube);
void checkCollisions(std::vector<Cube*> cubes, float c);
void calculateCollision(CollisionInfo hitInfo, Cube* cube_1, Cube* cube_2, float c);
Cube initializeCube(glm::vec3 x_cm_0, glm::vec3 dimensions, float M,
                    glm::vec3 rotation_0, glm::vec3 v_cm_0, glm::vec3 L_0);


#endif //RIGIDBODY_H
