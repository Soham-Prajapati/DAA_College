#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to allocate a square matrix
double **allocateMatrix(int n)
{
    double **matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (double *)calloc(n, sizeof(double));
    }
    return matrix;
}

// Function to free memory
void freeMatrix(double **matrix, int n)
{
    for (int i = 0; i < n; i++)
        free(matrix[i]);
    free(matrix);
}

// Fill matrix with random values
void fillRandom(double **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = (double)(rand() % 10);
        }
    }
}

// 1. ThreeLoop Triple Loop
void ThreeLoopMultiply(double **A, double **B, double **C, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < n; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Helper for D&C and Strassen: Matrix Addition/Subtraction
void add(double **A, double **B, double **C, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subtract(double **A, double **B, double **C, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// 2. Divide & Conquer (Recursive)
void divideAndConquer(double **A, double **B, double **C, int n)
{
    if (n <= 32)
    { // Base case: Use ThreeLoop for small matrices to avoid recursion overhead
        ThreeLoopMultiply(A, B, C, n);
        return;
    }

    int newSize = n / 2;
    double **a11 = allocateMatrix(newSize);
    double **a12 = allocateMatrix(newSize);
    double **a21 = allocateMatrix(newSize);
    double **a22 = allocateMatrix(newSize);
    double **b11 = allocateMatrix(newSize);
    double **b12 = allocateMatrix(newSize);
    double **b21 = allocateMatrix(newSize);
    double **b22 = allocateMatrix(newSize);

    double **c11 = allocateMatrix(newSize);
    double **c12 = allocateMatrix(newSize);
    double **c21 = allocateMatrix(newSize);
    double **c22 = allocateMatrix(newSize);
    double **temp1 = allocateMatrix(newSize);
    double **temp2 = allocateMatrix(newSize);

    for (int i = 0; i < newSize; i++)
    {
        for (int j = 0; j < newSize; j++)
        {
            a11[i][j] = A[i][j];
            a12[i][j] = A[i][j + newSize];
            a21[i][j] = A[i + newSize][j];
            a22[i][j] = A[i + newSize][j + newSize];
            b11[i][j] = B[i][j];
            b12[i][j] = B[i][j + newSize];
            b21[i][j] = B[i + newSize][j];
            b22[i][j] = B[i + newSize][j + newSize];
        }
    }

    // C11 = A11*B11 + A12*B21
    divideAndConquer(a11, b11, temp1, newSize);
    divideAndConquer(a12, b21, temp2, newSize);
    add(temp1, temp2, c11, newSize);

    // C12 = A11*B12 + A12*B22
    divideAndConquer(a11, b12, temp1, newSize);
    divideAndConquer(a12, b22, temp2, newSize);
    add(temp1, temp2, c12, newSize);

    // C21 = A21*B11 + A22*B21
    divideAndConquer(a21, b11, temp1, newSize);
    divideAndConquer(a22, b21, temp2, newSize);
    add(temp1, temp2, c21, newSize);

    // C22 = A21*B12 + A22*B22
    divideAndConquer(a21, b12, temp1, newSize);
    divideAndConquer(a22, b22, temp2, newSize);
    add(temp1, temp2, c22, newSize);

    for (int i = 0; i < newSize; i++)
    {
        for (int j = 0; j < newSize; j++)
        {
            C[i][j] = c11[i][j];
            C[i][j + newSize] = c12[i][j];
            C[i + newSize][j] = c21[i][j];
            C[i + newSize][j + newSize] = c22[i][j];
        }
    }

    // Clean up
    freeMatrix(a11, newSize);
    freeMatrix(a12, newSize);
    freeMatrix(a21, newSize);
    freeMatrix(a22, newSize);
    freeMatrix(b11, newSize);
    freeMatrix(b12, newSize);
    freeMatrix(b21, newSize);
    freeMatrix(b22, newSize);
    freeMatrix(c11, newSize);
    freeMatrix(c12, newSize);
    freeMatrix(c21, newSize);
    freeMatrix(c22, newSize);
    freeMatrix(temp1, newSize);
    freeMatrix(temp2, newSize);
}

// 3. Strassen's Algorithm
void strassen(double **A, double **B, double **C, int n)
{
    if (n <= 32)
    {
        ThreeLoopMultiply(A, B, C, n);
        return;
    }

    int newSize = n / 2;
    double **a11 = allocateMatrix(newSize);
    double **a12 = allocateMatrix(newSize);
    double **a21 = allocateMatrix(newSize);
    double **a22 = allocateMatrix(newSize);
    double **b11 = allocateMatrix(newSize);
    double **b12 = allocateMatrix(newSize);
    double **b21 = allocateMatrix(newSize);
    double **b22 = allocateMatrix(newSize);

    for (int i = 0; i < newSize; i++)
    {
        for (int j = 0; j < newSize; j++)
        {
            a11[i][j] = A[i][j];
            a12[i][j] = A[i][j + newSize];
            a21[i][j] = A[i + newSize][j];
            a22[i][j] = A[i + newSize][j + newSize];
            b11[i][j] = B[i][j];
            b12[i][j] = B[i][j + newSize];
            b21[i][j] = B[i + newSize][j];
            b22[i][j] = B[i + newSize][j + newSize];
        }
    }

    double **m1 = allocateMatrix(newSize);
    double **m2 = allocateMatrix(newSize);
    double **m3 = allocateMatrix(newSize);
    double **m4 = allocateMatrix(newSize);
    double **m5 = allocateMatrix(newSize);
    double **m6 = allocateMatrix(newSize);
    double **m7 = allocateMatrix(newSize);
    double **t1 = allocateMatrix(newSize);
    double **t2 = allocateMatrix(newSize);

    // M1 = (A11 + A22)(B11 + B22)
    add(a11, a22, t1, newSize);
    add(b11, b22, t2, newSize);
    strassen(t1, t2, m1, newSize);
    // M2 = (A21 + A22)B11
    add(a21, a22, t1, newSize);
    strassen(t1, b11, m2, newSize);
    // M3 = A11(B12 - B22)
    subtract(b12, b22, t2, newSize);
    strassen(a11, t2, m3, newSize);
    // M4 = A22(B21 - B11)
    subtract(b21, b11, t2, newSize);
    strassen(a22, t2, m4, newSize);
    // M5 = (A11 + A12)B22
    add(a11, a12, t1, newSize);
    strassen(t1, b22, m5, newSize);
    // M6 = (A21 - A11)(B11 + B12)
    subtract(a21, a11, t1, newSize);
    add(b11, b12, t2, newSize);
    strassen(t1, t2, m6, newSize);
    // M7 = (A12 - A22)(B21 + B22)
    subtract(a12, a22, t1, newSize);
    add(b21, b22, t2, newSize);
    strassen(t1, t2, m7, newSize);

    // C11 = M1 + M4 - M5 + M7
    add(m1, m4, t1, newSize);
    subtract(t1, m5, t2, newSize);
    add(t2, m7, t1, newSize);
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++)
            C[i][j] = t1[i][j];

    // C12 = M3 + M5
    add(m3, m5, t1, newSize);
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++)
            C[i][j + newSize] = t1[i][j];

    // C21 = M2 + M4
    add(m2, m4, t1, newSize);
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++)
            C[i + newSize][j] = t1[i][j];

    // C22 = M1 - M2 + M3 + M6
    subtract(m1, m2, t1, newSize);
    add(t1, m3, t2, newSize);
    add(t2, m6, t1, newSize);
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++)
            C[i + newSize][j + newSize] = t1[i][j];

    // Free all temps
    freeMatrix(a11, newSize);
    freeMatrix(a12, newSize);
    freeMatrix(a21, newSize);
    freeMatrix(a22, newSize);
    freeMatrix(b11, newSize);
    freeMatrix(b12, newSize);
    freeMatrix(b21, newSize);
    freeMatrix(b22, newSize);
    freeMatrix(m1, newSize);
    freeMatrix(m2, newSize);
    freeMatrix(m3, newSize);
    freeMatrix(m4, newSize);
    freeMatrix(m5, newSize);
    freeMatrix(m6, newSize);
    freeMatrix(m7, newSize);
    freeMatrix(t1, newSize);
    freeMatrix(t2, newSize);
}

int main()
{
    srand(time(NULL));
    FILE *fp = fopen("results.csv", "w");
    fprintf(fp, "N,ThreeLoop,DivideAndConquer,Strassen\n");

    printf("Starting Matrix Multiplication Comparison...\n");
    printf("N\tThreeLoop(s)\tD&C(s)\t\tStrassen(s)\n");
    printf("----------------------------------------------------------\n");

    for (int n = 2; n <= 1024; n *= 2)
    {
        double **A = allocateMatrix(n);
        double **B = allocateMatrix(n);
        double **C = allocateMatrix(n);

        fillRandom(A, n);
        fillRandom(B, n);

        clock_t start, end;
        double t_ThreeLoop, t_dc, t_strassen;

        // ThreeLoop
        start = clock();
        ThreeLoopMultiply(A, B, C, n);
        end = clock();
        t_ThreeLoop = (double)(end - start) / CLOCKS_PER_SEC;

        // D&C
        start = clock();
        divideAndConquer(A, B, C, n);
        end = clock();
        t_dc = (double)(end - start) / CLOCKS_PER_SEC;

        // Strassen
        start = clock();
        strassen(A, B, C, n);
        end = clock();
        t_strassen = (double)(end - start) / CLOCKS_PER_SEC;

        printf("%d\t%f\t%f\t%f\n", n, t_ThreeLoop, t_dc, t_strassen);
        fprintf(fp, "%d,%f,%f,%f\n", n, t_ThreeLoop, t_dc, t_strassen);

        freeMatrix(A, n);
        freeMatrix(B, n);
        freeMatrix(C, n);
    }

    fclose(fp);
    printf("----------------------------------------------------------\n");
    printf("Results saved to results.csv\n");
    return 0;
}