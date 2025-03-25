#include <cmath>
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
