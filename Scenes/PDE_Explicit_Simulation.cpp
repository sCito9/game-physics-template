//
// Created by felix on 10.12.2025.
//

#include "PDE_Explicit_Simulation.h"


#include "imgui.h"

void PDE_Explicit_Simulation::onDraw(Renderer& renderer)
{
    std::vector<float> data;
    for (auto& row : M)
    {
        data.insert(data.end(), row.begin(), row.end());
    }

    renderer.drawImage(data, M.size(), M[0].size());
}

void PDE_Explicit_Simulation::onGUI()
{
    ImGui::SliderFloat("Diffusity", &v, 0.01f, 1.f);
    ImGui::SliderFloat("time step", &delta_t, 0.001f, 1.f);

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


void PDE_Explicit_Simulation::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;
    cur_delta_time += realtimeDt;

    for (; cur_delta_time >= delta_t; cur_delta_time -= delta_t)
    {
        solveExplicitMatrix(M);
    }
}

void PDE_Explicit_Simulation::init()
{
    initializeRandomNoise();
}

void PDE_Explicit_Simulation::solveExplicitMatrix(std::vector<std::vector<float>> M_old)
{
    for (int i = 1; i < M_old.size() - 1; ++i)
    {
        for (int j = 1; j < M_old[0].size() - 1; ++j)
        {
            M[i][j] = solveExplicitHeatInteriorAt(i, j, M_old);
        }
    }
}


float PDE_Explicit_Simulation::solveExplicitHeatInteriorAt(int i, int j, std::vector<std::vector<float>> M_old)
{
    float part_1 = (1 - 2 * v * delta_t * (1.f / std::pow(delta_x, 2) + 1.f / std::pow(delta_y, 2))) * M_old[i][j];
    float part_2 = ((v * delta_t) / std::pow(delta_x, 2)) * (M_old[i + 1][j] + M_old[i - 1][j]);
    float part_3 = ((v * delta_t) / std::pow(delta_y, 2)) * (M_old[i][j + 1] + M_old[i][j - 1]);
    return part_1 + part_2 + part_3;
}

void PDE_Explicit_Simulation::initializeRandomNoise(float mean, float stddev)
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
