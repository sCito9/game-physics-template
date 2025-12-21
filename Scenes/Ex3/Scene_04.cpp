#include "Scene_04.h"

void Scene_04::init() {
    Scene::init();
}

void Scene_04::onDraw(Renderer &renderer) {
    T_t.viz(renderer);
}

void Scene_04::simulateStep() {
    if (paused || deltaT <= 0.f) return;

    if (!realTime) {
        (T_t.*Scene_04::stepFunc)();
        return;
    }
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

    ImGui::LabelText("Info", "[Space] to %sPAUSE sim\nRandbedingungen mitgerendert",  paused ? "UN" : "");
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        paused = !paused;
    }
    if (ImGui::Button("reinitialize")) {
        T_t.initializeHeatFieldAsGaussianBlob();
        paused = true;
    }
    if (ImGui::InputFloat("delta t in s", &deltaT, 0, 0, "%.5f")) {
        T_t.delta_t = deltaT;
    }
    ImGui::Checkbox("use real time", &realTime);
    ImGui::InputFloat("sim speed\nreal time", &speed);

    ImGui::InputFloat("heat on\nright click", &inputHeat);
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        ImVec2 mPos = ImGui::GetMousePos();
        ImVec2 size = ImGui::GetIO().DisplaySize;

        float j_rel = (mPos.x / size.x);
        auto j = (int)(j_rel * (float)n);

        float i_rel = (mPos.y / size.y);
        auto i = (int)(i_rel * (float)m);

        if (i <= 0 || i >= m - 1 || j <= 0 || j >= n - 1) return;

        T_t(i, j) = inputHeat;
    }

    ImGui::InputFloat("diffusivity\nny", &T_t.ny);
    if (ImGui::InputFloat("extent x", &width)) {
        T_t.setX(width);
    }
    if (ImGui::InputFloat("extent y", &height)) {
        T_t.setY(height);
    }
    int newM = (int)m - 2; // anzeige ohne randbed.
    if (ImGui::SliderInt("res m", &newM, 3, 665)) {
        m = newM + 2;
        T_t.setM(m);
    }
    int newN = (int)n - 2;
    if (ImGui::SliderInt("res n", &newN, 3, 665)) {
        n = newN + 2;
        T_t.setN(n);
    }
}

Scene_04::Scene_04() : width(1.0f), height(1.0f), n(18), m(18) {
    T_t = Heat(n, m, width, height, deltaT, 0.1f,true);
    scheme = implic;
    stepFunc = &Heat::implicitEuler;
}
