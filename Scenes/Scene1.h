//
// Created by tonib on 28.11.2025.
//

#ifndef SCENE1_H
#define SCENE1_H
#include "Scene.h"


class Scene1 : public Scene {
void init() override;
    glm::vec3 getPoint_x_pos(Rigidbody_Cube* cube);
    glm::vec3 getPoint_x_vel(Rigidbody_Cube* cube);
};



#endif //SCENE1_H
