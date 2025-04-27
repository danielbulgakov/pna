#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <omp.h>

void Cholesky_Decomposition(double* A, double* L, int n) {
    int blockSize = 32;

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            L[i * n + j] = 0.0;

    for (int k = 0; k < n; k += blockSize) {
        int end = std::min(k + blockSize, n);

        for (int i = k; i < end; ++i) {
            double sum = 0.0;
            for (int p = 0; p < i; ++p)
                sum += L[i * n + p] * L[i * n + p];
            L[i * n + i] = std::sqrt(A[i * n + i] - sum);

            for (int j = i + 1; j < end; ++j) {
                sum = 0.0;
                for (int p = 0; p < i; ++p)
                    sum += L[j * n + p] * L[i * n + p];
                L[j * n + i] = (A[j * n + i] - sum) / L[i * n + i];
            }
        }

        #pragma omp parallel for collapse(2)
        for (int i = end; i < n; ++i) {
            for (int j = k; j < end; ++j) {
                double sum = 0.0;
                for (int p = 0; p < j; ++p)
                    sum += L[i * n + p] * L[j * n + p];
                L[i * n + j] = (A[i * n + j] - sum) / L[j * n + j];
            }
        }
    }
}

void TestCholesky(int n) {
    // Создаем случайную положительно определенную матрицу
    std::vector<double> A(n * n);
    for (int i = 0; i < n * n; ++i) {
        A[i] = (rand() % 100) + 1.0;
    }

    // Копируем матрицу для разложения
    std::vector<double> L(n * n);
    
    // Измеряем время
    auto start = std::chrono::high_resolution_clock::now();
    Cholesky_Decomposition(A.data(), L.data(), n);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Matrix size: " << n << "x" << n << " took " << duration.count() << " seconds." << std::endl;
}

int main() {
    std::vector<int> sizes = {1000, 3000, 5000};
    std::vector<int> threads = {1, 2, 4, 6};

    for (int numThreads : threads) {
        omp_set_num_threads(numThreads);
        std::cout << "Testing with " << numThreads << " threads" << std::endl;

        for (int size : sizes) {
            TestCholesky(size);
        }
        std::cout << std::endl;
    }

    return 0;
}
