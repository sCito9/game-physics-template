//
// Created by felix on 12.12.2025.
//

#include "PDE_Implicit_Simulation.h"


#include "imgui.h"

void PDE_Implicit_Simulation::onDraw(Renderer& renderer)
{
    std::vector<float> data;
    for (auto& row : M)
    {
        data.insert(data.end(), row.begin(), row.end());
    }

    renderer.drawImage(data, M.size(), M[0].size());
}

void PDE_Implicit_Simulation::onGUI()
{
    auto slider_v = ImGui::SliderFloat("Diffusity", &v, 0.01f, 1.f);
    if (slider_v)
    {
        a_x = (v * delta_t) / glm::pow(delta_x, 2);
        a_y = (v * delta_t) / glm::pow(delta_y, 2);
        a = 1 + 2 * (a_x + a_y);
    }

    auto slider_t = ImGui::SliderFloat("time step", &delta_t, 0.001f, 1.f);
    if (slider_t)
    {
        a_x = (v * delta_t) / glm::pow(delta_x, 2);
        a_y = (v * delta_t) / glm::pow(delta_y, 2);
        a = 1 + 2 * (a_x + a_y);
    }

    auto button = ImGui::Button("Print values");
    if (button)
    {
        std::cout << "Heatmap M: " << std::endl;
        for (auto row : M)
        {
            for (auto value : row)
            {
                std::cout << value << " , ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void PDE_Implicit_Simulation::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;
    cur_delta_time += realtimeDt;

    for (; cur_delta_time >= delta_t; cur_delta_time -= delta_t)
    {
        solveImplicitMatrix(M);
    }
}

void PDE_Implicit_Simulation::init()
{
    initializeRandomNoise();
    sparse_matrix = SparseMatrix<float>(M.size() * M[0].size()); //fix expected nonzeros
    sparse_matrix.zero();
    rhs_b = std::vector<float>(M.size() * M[0].size(), 0.f);
    resetRhsB();
    result = std::vector<float>(M.size() * M[0].size(), 0.f);
}

void PDE_Implicit_Simulation::resetRhsB()
{
    int counter = 0;
    for (auto row : M)
    {
        for (auto value : row)
        {
            rhs_b[counter++] = value;
        }
    }
}


void PDE_Implicit_Simulation::solveImplicitMatrix(std::vector<std::vector<float>> M_old)
{
    sparse_matrix.zero();
    resetRhsB();
    zero(result);
    //iterate full matrix because edge point matter
    for (int i = 0; i < M_old.size(); ++i)
    {
        for (int j = 0; j < M_old[0].size(); ++j)
        {
            solveImplicitHeatInteriorAt(i, j, M_old);
            enforceBoundaries();
        }
    }
    bool solve_result = sparse_solver.solve(sparse_matrix, rhs_b, result, final_residual, iterations_out);
    if (solve_result)
    {
        int count = 0;
        for (int i = 0; i < M.size(); ++i)
        {
            for (int j = 0; j < M[0].size(); ++j)
            {
                M[i][j] = result[count++];
            }
        }
    }
}

void PDE_Implicit_Simulation::solveImplicitHeatInteriorAt(int i, int j, std::vector<std::vector<float>> M_old)
{
    int k = j + i * M_old[0].size();

    //rhs_b muss nicht geupdated werden, da alle randpunkte 0 sind und das in der rechnung auch zu einer 0 führt

    //Randpunkt
    if (i == 0 || i == M_old.size() - 1 || j == 0 || j == M_old[0].size() - 1)
    {
        sparse_matrix.set_element(k, k, 1.f);
        return;
    }

    sparse_matrix.set_element(k, k, a);

    //oben
    if (i > 0)
    {
        sparse_matrix.set_element(k, j + (i - 1) * M_old[0].size(), -a_y);
    }
    //unten
    if (i < M_old.size() - 1)
    {
        sparse_matrix.set_element(k, j + (i + 1) * M_old[0].size(), -a_y);
    }
    //rechts
    if (j > 0)
    {
        sparse_matrix.set_element(k, (j - 1) + i * M_old[0].size(), -a_x);
    }
    //links
    if (j < M_old[0].size() - 1)
    {
        sparse_matrix.set_element(k, (j + 1) + i * M_old[0].size(), -a_x);
    }
}

void PDE_Implicit_Simulation::initializeRandomNoise(float mean, float stddev)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(mean, stddev);

    for (int i = 1; i < M.size() - 1; i++)
    {
        for (int j = 1; j < M[0].size() - 1; j++)
        {
            M[i][j] = dist(gen);
        }
    }
}

void PDE_Implicit_Simulation::enforceBoundaries()
{
    if (M.empty() || M[0].empty())
        return;

    for (int j = 0; j < M[0].size(); ++j)
    {
        M[0][j] = 0.f;
        M[M.size() - 1][j] = 0.f;
    }

    for (int i = 0; i < M.size(); ++i)
    {
        M[i][0] = 0.f;
        M[i][M[0].size() - 1] = 0.f;
    }
}
