#include "Scene_04.h"

void Scene_04::init() {
    Scene::init();
}

void Scene_04::onDraw(Renderer &renderer) {
    T_t.viz(renderer);
}

void Scene_04::simulateStep() {
    if (paused || deltaT <= 0.f) return;
    float realtimeDt = ImGui::GetIO().DeltaTime * speed;

    curDeltaT += realtimeDt;
    for (; curDeltaT >= deltaT; curDeltaT -= deltaT) {
        (T_t.*Scene_04::stepFunc)();
    }
}

void Scene_04::onGUI() {
    if (ImGui::Selectable("Explicit", scheme == explic)) {
        scheme = explic;
        stepFunc = &Heat::explicitEuler;
    }
    if (ImGui::Selectable("Implicit", scheme == implic)) {
        scheme = implic;
        stepFunc = &Heat::implicitEuler;
    }

    ImGui::LabelText("Info", "[Space] to (un-)pause sim");
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        paused = !paused;
    }
    ImGui::InputFloat("sim speed", &speed);
    if (ImGui::InputFloat("delta t in s", &deltaT)) {
        T_t.delta_t = deltaT;
    }

    ImGui::InputFloat("heat on rclick", &inputHeat);
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        ImVec2 mPos = ImGui::GetMousePos();
        ImVec2 size = ImGui::GetIO().DisplaySize;
        float j = (mPos.x / size.x);
        float i = (mPos.y / size.y);
        std::cout << i << ", " << j << std::endl;

        if (i < 0 || i > 1 || j < 0 || j > 1) return;

        T_t((int)(i * (float)m), (int)(j * (float)n)) = inputHeat;
    }

    ImGui::InputFloat("visc. ny", &T_t.ny);
    if (ImGui::InputFloat("extent x", &width)) {
        T_t.setX(width);
    }
    if (ImGui::InputFloat("extent y", &height)) {
        T_t.setY(height);
    }
    int newM = (int)m;
    if (ImGui::InputInt("res m", &newM)) {
        T_t.setM(newM);
        m = newM;
    }
    int newN = (int)n;
    if (ImGui::InputInt("res n", &newN)) {
        T_t.setN(newN);
        n = newN;
    }
}

Scene_04::Scene_04() : width(1.0f), height(1.0f), n(18), m(18) {
    T_t = Heat(n, m, width, height, deltaT, 0.1f,true);
    scheme = implic;
    stepFunc = &Heat::implicitEuler;
}
