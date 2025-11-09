//
// Created by tonib on 09.11.2025.
//

#ifndef SCENE4_H
#define SCENE4_H
#include "Scene.h"


class Scene4 : public Scene{
public:

    void init() override;

    void ShowDropdownMenuForMethods();

    void onGUI() override;

    virtual void simulateStep() override;
};



#endif //SCENE4_H
