//
// Created by tonib on 09.11.2025.
//

#ifndef SCENE2_H
#define SCENE2_H
#include "MassPoint.h"
#include "Scene.h"


class Scene2 : public Scene{
    public:

    void init() override;

    virtual void onGUI() override;

    virtual void simulateStep() override;
};



#endif //SCENE2_H
