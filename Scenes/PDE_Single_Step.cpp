//
// Created by felix on 10.12.2025.
//

#include "PDE_Single_Step.h"

void PDE_Single_Step::onDraw(Renderer& renderer)
{
    std::vector<float> data;
    for (auto& row : M)
    {
        data.insert(data.end(), row.begin(), row.end());
    }

    renderer.drawImage(data, M.size(), M[0].size());
}


void PDE_Single_Step::init()
{
    solveExplicitMatrix(M);
    std::cout << "T1, 3: " << M[2][4] << ", T0, 3: " << M[1][4] << ", T0, 5: " << M[1][6] << std::endl;
}

void PDE_Single_Step::solveExplicitMatrix(std::vector<std::vector<float>> M_old)
{
    for (int i = 1; i < M_old.size() - 1; ++i)
    {
        for (int j = 1; j < M_old[0].size() - 1; ++j)
        {
            M[i][j] = solveExplicitHeatInteriorAt(i, j, M_old);
        }
    }
}


float PDE_Single_Step::solveExplicitHeatInteriorAt(int i, int j, std::vector<std::vector<float>> M_old)
{
    float part_1 = (1 - 2 * v * delta_t * (1.f / std::pow(delta_x, 2) + 1.f / std::pow(delta_y, 2))) * M_old[i][j];
    float part_2 = ((v * delta_t) / std::pow(delta_x, 2)) * (M_old[i + 1][j] + M_old[i - 1][j]);
    float part_3 = ((v * delta_t) / std::pow(delta_y, 2)) * (M_old[i][j + 1] + M_old[i][j - 1]);
    return part_1 + part_2 + part_3;
}

