//
// Created by felix on 13.12.2025.
//

#include "PDE_Interactive_Simulation.h"

#include <bits/random.h>

#include "imgui.h"

void PDE_Interactive_Simulation::onDraw(Renderer& renderer)
{
    if (M.empty() || M[0].empty())
        return;
    std::vector<float> data;
    for (auto& row : M)
    {
        data.insert(data.end(), row.begin(), row.end());
    }

    renderer.drawImage(data, M.size(), M[0].size());
}

void PDE_Interactive_Simulation::onGUI()
{
    const char* items[] = {"Implicit Integration", "Explicit Integration"};
    int current_index = static_cast<int>(currentState);

    if (ImGui::Combo("Method", &current_index, items, 2))
    {
        currentState = static_cast<State>(current_index);
    }

    bool resolutionChanged = ImGui::InputInt2("Grid Resolution m x n: ", grid_resolution);
    if (resolutionChanged)
    {
        pending_resize = true;
    }

    bool domainChanged = ImGui::InputFloat2("Domain Extent: ", domain_extent);
    if (domainChanged)
    {
        delta_x = domain_extent[0] / (M[0].size());
        delta_y = domain_extent[1] / (M.size());

        a_x = (v * delta_t) / glm::pow(delta_x, 2);
        a_y = (v * delta_t) / glm::pow(delta_y, 2);
        a = 1 + 2 * (a_x + a_y);
    }

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

    ImGui::SliderFloat("Temperature add when right klicking: ", &temperature_addition, 0.f, 100.f);
    ImGui::InputInt2("Grid Position Heat by Mouse", temperature_spread);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        if (M.empty() || M[0].empty())
            return;
        auto mouse_Position = ImGui::GetMousePos();

        float mouse_x = width / M[0].size();
        float mouse_y = height / M.size();

        float row_pos = mouse_Position.y / mouse_y;
        float column_pos = mouse_Position.x / mouse_x;

        row_pos = std::clamp(row_pos, 0.f, (float)M.size() - 2);
        column_pos = std::clamp(column_pos, 0.f, (float)M[0].size() - 2);

        int row_klicked = static_cast<int>(row_pos);
        int column_klicked = static_cast<int>(column_pos);

        //temporary
        row_klicked = std::clamp(temperature_spread[1], 1, (int)(M.size() - 2));
        column_klicked = std::clamp(temperature_spread[0], 1, (int)(M[0].size() - 2));

        M[row_klicked][column_klicked] += temperature_addition;
        std::cout << "row: " << row_klicked << ", column: " << column_klicked << std::endl;
    }
}

void PDE_Interactive_Simulation::simulateStep()
{
    if (pending_resize)
    {
        initializeMatrix(grid_resolution[0], grid_resolution[1], glm::vec2(domain_extent[0], domain_extent[1]));
        pending_resize = false;
        cur_delta_time = 0.f;
        return;
    }

    float realtimeDt = ImGui::GetIO().DeltaTime;
    cur_delta_time += realtimeDt;

    for (; cur_delta_time >= delta_t; cur_delta_time -= delta_t)
    {
        if (currentState == State::Implicit_Integration)
        {
            solveImplicitMatrix(M);
        }
        else
        {
            solveExplicitMatrix(M);
        }
        enforceBoundaries();
    }
}

void PDE_Interactive_Simulation::enforceBoundaries()
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


void PDE_Interactive_Simulation::init()
{
    initializeMatrix();
}

void PDE_Interactive_Simulation::resetRhsB()
{
    int required = M.size() * (M.empty() ? 0 : M[0].size());
    if (rhs_b.size() != required)
        rhs_b.assign(required, 0.f);

    int counter = 0;
    for (auto row : M)
    {
        for (auto value : row)
        {
            rhs_b[counter++] = value;
        }
    }
}


void PDE_Interactive_Simulation::solveImplicitMatrix(std::vector<std::vector<float>> M_old)
{
    if (M.empty() || M[0].empty())
        return;

    sparse_matrix.zero();
    resetRhsB();
    zero(result);
    //iterate full matrix because edge point matter
    for (int i = 0; i < M_old.size(); ++i)
    {
        for (int j = 0; j < M_old[0].size(); ++j)
        {
            solveImplicitHeatInteriorAt(i, j, M_old);
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

void PDE_Interactive_Simulation::solveImplicitHeatInteriorAt(int i, int j, std::vector<std::vector<float>> M_old)
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

void PDE_Interactive_Simulation::solveExplicitMatrix(std::vector<std::vector<float>> M_old)
{
    if (M_old.size() < 3 || M_old[0].size() < 3)
        return;

    for (int i = 1; i < M_old.size() - 1; ++i)
    {
        for (int j = 1; j < M_old[0].size() - 1; ++j)
        {
            M[i][j] = solveExplicitHeatInteriorAt(i, j, M_old);
        }
    }
}


float PDE_Interactive_Simulation::solveExplicitHeatInteriorAt(int i, int j, std::vector<std::vector<float>> M_old)
{
    float part_1 = (1 - 2 * v * delta_t * (1.f / std::pow(delta_x, 2) + 1.f / std::pow(delta_y, 2))) * M_old[i][j];
    float part_2 = ((v * delta_t) / std::pow(delta_x, 2)) * (M_old[i + 1][j] + M_old[i - 1][j]);
    float part_3 = ((v * delta_t) / std::pow(delta_y, 2)) * (M_old[i][j + 1] + M_old[i][j - 1]);
    return part_1 + part_2 + part_3;
}

void PDE_Interactive_Simulation::initializeRandomNoise(float mean, float stddev)
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

void PDE_Interactive_Simulation::initializeMatrix(int rows, int columns, glm::vec2 domain)
{
    M.resize(rows);
    for (auto& row : M)
    {
        row.resize(columns, 0.0f);
    }
    delta_x = domain.x / (float)columns;
    delta_y = domain.y / (float)rows;
    initializeRandomNoise();

    //implicit stuff
    sparse_matrix = SparseMatrix<float>(M.size() * M[0].size()); //fix expected nonzeros
    sparse_matrix.zero();
    rhs_b = std::vector<float>(M.size() * M[0].size(), 0.f);
    resetRhsB();
    result = std::vector<float>(M.size() * M[0].size(), 0.f);
    sparse_solver = SparsePCGSolver<float>();

    a_x = (v * delta_t) / glm::pow(delta_x, 2);
    a_y = (v * delta_t) / glm::pow(delta_y, 2);
    a = 1 + 2 * (a_x + a_y);
}

