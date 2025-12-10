//
// Created by tonib on 10.12.2025.
//

#ifndef SCENE5_H
#define SCENE5_H
#include "Scene.h"


class Scene4 : public Scene {
    void init() override;
    void simulateStep() override;
    void onGUI() override;

    void addImplosionForces();
    bool activeGravity = false;
};



#endif //SCENE5_H
