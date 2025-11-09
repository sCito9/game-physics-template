//
// Created by tonib on 09.11.2025.
//

#ifndef SCENE3_H
#define SCENE3_H
#include "Scene.h"


class Scene3 : public Scene {
    public:

    void init() override;

    void onGUI() override;

    virtual void simulateStep() override;
};



#endif //SCENE3_H
