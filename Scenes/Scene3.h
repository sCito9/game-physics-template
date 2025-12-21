//
// Created by tonib on 17.12.2025.
//

#ifndef SCENE3_H
#define SCENE3_H
#include "Scene.h"


class Scene3 : public Scene{
    void init() override;
    void onGUI() override;
    void simulateStep() override;
};



#endif //SCENE3_H
