/*
* Sum of two vectors
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>

void add(double* a, double* b, double* c, size_t n)
{
    for (size_t i = 0; i <= n; ++i) {
        c[i] = a[i] + b[i];
    }
}

void addpd(double* a, double* b, double* c, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        const __m256d apd = _mm256_load_pd(a);
        const __m256d bpd = _mm256_load_pd(b);
        const __m256d sum = _mm256_add_pd(apd, bpd);
        _mm256_store_pd(c, sum);

        a += 4;
        b += 4;
        c += 4;
    }
}

void randarr(double* a, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        a[i] = rand() % n;
    }
}

void print(double* a, size_t n)
{
    printf("[");

    for (size_t i = 0; i < n - 1; ++i) {
        printf("%f, ", a[i]);
    }

    printf("%f]\n", a[n - 1]);
}

int equal(double* a, double* b, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
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

    print(a, n);
    print(b, n);

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

    print(sum1, n);
    print(sum2, n);

    return 0;
}