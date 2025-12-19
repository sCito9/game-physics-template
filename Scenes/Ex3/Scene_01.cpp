#include "Scene_01.h"

void Scene_01::init() {

    T_t(1,1) = 6;
    T_t(1,2) = 5;
    T_t(1,3) = 1;
    T_t(1,4) = -1;
    T_t(1,5) = -2;
    T_t(1,6) = -1;

    T_t(2,1) = 4;
    T_t(2,2) = 3;
    T_t(2,3) = 0;
    T_t(2,4) = -1;
    T_t(2,5) = -3;
    T_t(2,6) = -1;

    T_t(3,1) = 3;
    T_t(3,2) = 2;
    T_t(3,3) = -1;
    T_t(3,4) = -2;
    T_t(3,5) = -4;
    T_t(3,6) = -2;

    std::cout << "t=0 : T_1,3 = " << T_t(2,4)
    << "  T_0,3 = " << T_t(1,4) << "  T_0,5 = " << T_t(1,6) << std::endl;

    T_t.explicitEuler();

    std::cout << "t=1 : T_1,3 = " << T_t(2,4)
    << "  T_0,3 = " << T_t(1,4) << "  T_0,5 = " << T_t(1,6) << std::endl;
}

void Scene_01::onDraw(Renderer &renderer) {
    Scene::onDraw(renderer);
}

void Scene_01::simulateStep() {
    Scene::simulateStep();
}

void Scene_01::onGUI() {
    Scene::onGUI();
}

Scene_01::Scene_01() : width(2.0f), height(4.0f) {
    uint32_t n = 8; // 6+2
    uint32_t m = 5; // 3+2
    float dx = width / (float)(n - 1);
    float dy = height / (float)(m - 1);
    std::cout << "delta_x = " << dx <<"\ndelta y = " << dy << std::endl;

    T_t = Heat(n, m, dx, dy, 0.1f, 0.1f,false);
}
