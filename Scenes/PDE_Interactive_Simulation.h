//
// Created by felix on 13.12.2025.
//

#ifndef PDE_INTERACTIVE_SIMULATION_H
#define PDE_INTERACTIVE_SIMULATION_H
#include "Scene.h"
#include "util/pcgsolver.h"


class PDE_Interactive_Simulation : public Scene
{
    float v = 0.1f;
    float delta_t = 0.01f;
    float cur_delta_time = 0.f;

    std::vector<std::vector<float>> M{};
    int grid_resolution[2] = {18, 18};
    float domain_extent[2] = {2.f, 2.f};
    int temperature_spread[2] = {1, 1};

    float delta_x = 1.f;
    float delta_y = 1.f;

    SparsePCGSolver<float> sparse_solver = SparsePCGSolver<float>();
    SparseMatrix<float> sparse_matrix{};
    std::vector<float> rhs_b{};
    std::vector<float> result{};
    float final_residual = -1.f;
    int iterations_out = -1;

    float a_x = 1.f;
    float a_y = 1.f;
    float a = 1.f;

    float temperature_addition = 5.f;

    bool pending_resize = false;


    enum class State { Implicit_Integration, Explicit_Integration };

    State currentState = State::Implicit_Integration;

    //Methods

    void solveImplicitHeatInteriorAt(int i, int j, std::vector<std::vector<float>> M_old);
    void solveImplicitMatrix(std::vector<std::vector<float>> M_old);
    float solveExplicitHeatInteriorAt(int i, int j, std::vector<std::vector<float>> M_old);
    void solveExplicitMatrix(std::vector<std::vector<float>> M_old);
    void initializeRandomNoise(float mean = 0.0f, float stddev = 1.0f);
    void resetRhsB();
    void enforceBoundaries();
    void initializeMatrix(int rows = 18, int columns = 18, glm::vec2 domain = glm::vec2(2.f));
    void init() override;
    void onDraw(Renderer& renderer) override;
    void onGUI() override;
    void simulateStep() override;
};


#endif //PDE_INTERACTIVE_SIMULATION_H
