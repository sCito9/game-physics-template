#include "Heat.h"

/// @brief perform a singular explicit euler step
void Heat::explicitEuler() {
    const Heat T_t = *this;
    for (int i = 1; i < m - 1; i++) {
        for (int j = 1; j < n - 1; j++) {
            float diff1 = (T_t(i + 1, j) - (2 * T_t(i, j)) + T_t(i - 1, j)) / (delta_x * delta_x);
            float diff2 = (T_t(i, j + 1) - (2 * T_t(i, j)) + T_t(i, j - 1)) / (delta_y * delta_y);

            float differenzialquotient = ny * (diff1 + diff2);

            // T_t+1 =
            T_buffer[n * i + j] = differenzialquotient * delta_t + T_t(i, j);
        }
    }

    std::swap(T, T_buffer);
}

/// @brief perform a singular implicit euler step
void Heat::implicitEuler() {
    auto solver = SparsePCGSolver<float>();
    float residuum;
    int nIter;

    auto A = SparseMatrix<float>((int)size, 5);

    float lambda_x = ny * delta_t / (delta_x * delta_x);
    float lambda_y = ny * delta_t / (delta_y * delta_y);
    float diag = 1.0f + 2 * lambda_x + 2 * lambda_y;

    for (int i = 1, j = 1, k = (int)n + 1; k < size - (n + 1); k++, j++) {
        if (j >= n) {
            j = 0;
            i++;
            A.set_element(k ,k, 1);
            continue;
        }

        if (k == i * n + n - 1) {
            A.set_element(k ,k, 1);
            continue;
        }

        A.set_element(k, k, diag);
        A.set_element(k, k - 1, -lambda_y);
        A.set_element(k, k + 1, -lambda_y);
        A.set_element(k, k - n, -lambda_x);
        A.set_element(k, k + n, -lambda_x);
    }

    for (int k = 0; k < n + 1; k++) {
        A.set_element(k, k, 1);
        A.set_element((int)size - k - 1, (int)size - k - 1, 1);
    }

    if (!solver.solve(A, T, T_buffer, residuum, nIter)) {
        std::cout << "Solver not successful. res=" << residuum << ", nIter=" << nIter << std::endl;
    }

    std::swap(T, T_buffer);

    if (guaranteeBoundary) cleanBoundary();
}

void Heat::initializeHeatFieldAsGaussianBlob() {
    float x0 = 0.5f * x;
    float y0 = 0.5f * y;

    for (int j = 1; j < n; j++) {
        for (int i = 1; i < m; i++) {
            float x_cur = (float) j * delta_x;
            float y_cur = (float) i * delta_y;
            T[n * i + j] = /* Amplitude */ 1.0f * std::exp(
                    -((x_cur - x0) * (x_cur - x0) / (2 * sig_x * sig_x))
                    - ((y_cur - y0) * (y_cur - y0) / (2 * sig_y * sig_y))
            );
        }
    }
}

void Heat::cleanBoundary() {
    memset(T.data(), 0, n * sizeof(float));
    memset(T.data() + n * (m - 1), 0, n * sizeof(float));
    for (int i = 1; i < m - 1; i++) {
        for (int j = 0; j < n; j++) {
            if (j == 0 || j == n - 1) {
                T[n * i + j] = 0;
            }
        }
    }
}

void Heat::viz(Renderer &renderer) {
    renderer.drawImage(T, (int)m, (int)n);
}


