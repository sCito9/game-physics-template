#include "Scene_02.h"

void Scene_02::init() {
    Scene::init();
}

void Scene_02::onDraw(Renderer &renderer) {
    T_t.viz(renderer);
}

void Scene_02::simulateStep() {
    if (paused || deltaT <= 0.f) return;
    float realtimeDt = ImGui::GetIO().DeltaTime * speed;

    curDeltaT += realtimeDt;
    for (; curDeltaT >= deltaT; curDeltaT -= deltaT) {
        T_t.explicitEuler();
    }
}

void Scene_02::onGUI() {
    ImGui::LabelText("Info", "[Space] to (un-)pause sim");
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        paused = !paused;
    }
    ImGui::InputFloat("sim speed", &speed);
    if (ImGui::InputFloat("delta t in s", &deltaT)) {
        T_t.delta_t = deltaT;
    }
    ImGui::InputFloat("visc. ny", &T_t.ny);
}

Scene_02::Scene_02() : width(1.0f), height(1.0f), n(18), m(18) {
    T_t = Heat(n, m, width, height, deltaT, 0.1f,true);
}
