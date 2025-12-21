#pragma once

#include "src/util/pcgsolver.h"
#include "Renderer.h"

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cmath>
#include <ostream>
#include <iostream>

class Heat {
private:
    /// @brief temperature field T (at time t)
    std::vector<float> T;
    /// @brief buffer for temperature field T at time t+1
    std::vector<float> T_buffer;
    /// @brief dimension in x
    uint32_t m;
    /// @brief dimension in y
    uint32_t n;
    /// @brief total number of T_i,j
    size_t size;
    /// @brief total domain size in x
    float x;
    /// @brief discrete step size in x
    float delta_x;
    /// @brief total domain size in y
    float y;
    /// @brief discrete step size in y
    float delta_y;

    float sig_x;
    float sig_y;

    void cleanBoundary();

public:
    /// @brief discrete step size in time
    float delta_t;
    /// @brief viscosity factor ν
    float ny;

    bool guaranteeBoundary = true;

    float& operator()(uint32_t i, uint32_t j) {
        assert(i < m && j < n);
        return T[n * i + j];
    }
    const float& operator()(uint32_t i, uint32_t j) const {
        assert(i < m && j < n);
        return T[n * i + j];
    }

    void explicitEuler();

    void implicitEuler();

    void viz(Renderer& renderer);

    void initializeHeatFieldAsGaussianBlob();

    void setM(uint32_t mNew) {
        size = mNew * n;
        std::vector<float> newT(size);
        T_buffer = std::vector<float>(size);

        for (int i = 1; i < mNew && i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                newT[n * i + j] = T[n * i + j];
            }
        }

        T = newT;
        m = mNew;
        delta_y = y / (float)(m - 1);
    }

    void setN(uint32_t nNew) {
        size = m * nNew;
        std::vector<float> newT(size);
        T_buffer = std::vector<float>(size);

        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n && j < nNew; ++j) {
                newT[nNew * i + j] = T[n * i + j];
            }
        }

        T = newT;
        n = nNew;
        delta_x = x / (float)(n - 1);
    }

    void setX(float xNew) {
        x = xNew;
        delta_x = x / (float)(n - 1);
        sig_x = 0.1f * x;
    }

    void setY(float yNew) {
        y = yNew;
        delta_y = y / (float)(m - 1);
        sig_y = 0.1f * y;
    }

    explicit Heat(uint32_t n=0, uint32_t m=0, float x=2.0f, float y=4.0f, float delta_t=0.1f, float ny=0.1f, bool initAsGaussBlob=false) :
    n(n), m(m), x(x), y(y), delta_t(delta_t), ny(ny) {
        size = n * m;
        delta_x = x / (float)(n - 1);
        delta_y = y / (float)(m - 1);

        T = std::vector<float>(size);
        if (initAsGaussBlob) {
            sig_x = 0.1f * x;
            sig_y = 0.1f * y;
            initializeHeatFieldAsGaussianBlob();
        }
        T_buffer = std::vector(T);
    }
};

