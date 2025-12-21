#include "Scene_03.h"

void Scene_03::init() {
    Scene::init();
}

void Scene_03::onDraw(Renderer &renderer) {
    T_t.viz(renderer);
}

void Scene_03::simulateStep() {
    if (paused || deltaT <= 0.f) return;

    if (!realTime) {
        T_t.implicitEuler();
        return;
    }
    float realtimeDt = ImGui::GetIO().DeltaTime * speed;

    curDeltaT += realtimeDt;
    for (; curDeltaT >= deltaT; curDeltaT -= deltaT) {
        T_t.implicitEuler();
    }
}

void Scene_03::onGUI() {
    ImGui::LabelText("Info", "[Space] to %sPAUSE sim\nRandbedingungen mitgerendert",  paused ? "UN" : "");
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        paused = !paused;
    }
    if (ImGui::InputFloat("delta t in s", &deltaT, 0, 0, "%.5f")) {
        T_t.delta_t = deltaT;
    }
    ImGui::Checkbox("use real time", &realTime);
    ImGui::InputFloat("sim speed\nreal time", &speed);
    ImGui::InputFloat("diffusivity\nny", &T_t.ny);
}

Scene_03::Scene_03() : width(1.0f), height(1.0f), n(18), m(18) {
    T_t = Heat(n, m, width, height, deltaT, 0.1f,true);
}
