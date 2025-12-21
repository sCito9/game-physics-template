//
// Created by tonib on 18.12.2025.
//

#ifndef SCENE4_H
#define SCENE4_H
#include "Scene.h"


class Scene4 : public Scene{
    void init() override;
    void onGUI() override;
    void simulateStep() override;
    void ShowDropdownMenuForMethods();
    float tempToAssignViaClick = 0;
    int dimensionsMax;
    bool coolerResizeVariety = false;
};



#endif //SCENE4_H
