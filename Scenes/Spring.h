//
// Created by tonib on 08.11.2025.
//

#ifndef SPRING_H
#define SPRING_H
#include "MassPoint.h"


class Spring {

    public:
        float stiffness;
        float restLength;

        MassPoint* point1;
        MassPoint* point2;
        Spring(float initStiffness, float initRestLength, MassPoint* initPoint1, MassPoint* initPoint2);
        float getCurrentLength();
};



#endif //SPRING_H
