//
// Created by tonib on 14.12.2025.
//

#ifndef SCENE2_H
#define SCENE2_H
#include "Scene.h"


class Scene2 : public Scene{
    void init() override;
    void onGUI() override;
    void simulateStep() override;
};



#endif //SCENE2_H
