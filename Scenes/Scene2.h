//
// Created by tonib on 29.11.2025.
//

#ifndef SCENE2_H
#define SCENE2_H
#include "Scene.h"
#include "glm/detail/qualifier.hpp"


class Scene2 : public Scene {
    void init() override;
    void simulateStep() override;
    void onGUI() override;
};



#endif //SCENE2_H
