//
// Created by tonib on 29.11.2025.
//

#ifndef SCENE3_H
#define SCENE3_H
#include "Scene.h"


class Scene3 : public Scene{
    void init() override;
    void simulateStep() override;
    void onGUI() override;
};



#endif //SCENE3_H
