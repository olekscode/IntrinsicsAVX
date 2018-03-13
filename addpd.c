/*
* Sum of two vectors
*/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <immintrin.h>

void printarr(double* a, size_t n);

void add(double* a, double* b, double* c, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

void addpd(double* a, double* b, double* c, size_t n)
{
    for (size_t i = 0; i + 4 <= n; i += 4) {
        const __m256d apd = _mm256_load_pd(a + i);
        const __m256d bpd = _mm256_load_pd(b + i);
        const __m256d sum = _mm256_add_pd(apd, bpd);
        _mm256_store_pd(c + i, sum);
    }

    for (size_t i = n % 4; i > 0; --i) {
        c[n - i] = a[n - i] + b[n - i];
    }
}

void randarr(double* a, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        a[i] = rand() % 100;
    }
}

void printarr(double* a, size_t n)
{
    printf("[");

    for (size_t i = 0; i < n - 1; ++i) {
        printf("%f, ", a[i]);
    }

    printf("%f]\n", a[n - 1]);
}

int close(double a, double b)
{
    return fabs(a - b) < 10e-7;
}

int equal(double* a, double* b, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (!close(a[i], b[i])) {
            return 0;
        }
    }
    return 1;
}

void to_csv(int* col1, double* col2, double* col3, int n)
{
    FILE* fp = fopen("results.csv", "w");

    for (size_t i = 0; i < n; ++i) {
        fprintf(fp, "%d, %f, %f\n", col1[i], col2[i], col3[i]);
    }

    fclose(fp);
}

int main(int argc, char* argv[])
{
    srand(time(NULL));

    size_t m = 1000;

    int col1[m];
    double col2[m];
    double col3[m];

    for (int i = 0; i < m; ++i) {
        size_t n = (i + 1) * 100;
        col1[i] = n;

        double a[n];
        double b[n];
        double sum1[n];
        double sum2[n];

        randarr(a, n);
        randarr(b, n);

        time_t start, end;
        float seconds;

        start = clock();
        add(a, b, sum1, n);
        end = clock();
        seconds = (float)(end - start) / CLOCKS_PER_SEC;
        col2[i] = seconds;

        start = clock();
        addpd(a, b, sum2, n);
        end = clock();
        seconds = (float)(end - start) / CLOCKS_PER_SEC;
        col3[i] = seconds;

        // Otherwise it will be optimized
        printarr(sum1, n);
        printarr(sum2, n);
    }

    to_csv(col1, col2, col3, m);
    return 0;
}