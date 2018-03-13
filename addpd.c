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

int main(int argc, char* argv[])
{
    srand(time(NULL));

    size_t n = atoi(argv[1]);
    printf("Size of arrays: %zu\n", n);

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
    printf("Sequantial add: %f seconds\n", seconds);

    start = clock();
    addpd(a, b, sum2, n);
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("Vectorized add: %f seconds\n", seconds);

    printf("%d\n", equal(sum1, sum2, n));

    return 0;
}