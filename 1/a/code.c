#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* the functions to implement are as follows:
    1. n
    2. (3/2)^n
    3. n^3
    4. (log base 2 (n))^2
    5. log base 2 (n!)
    6. 2^(2n)
    7. log(log(n))
    8. log base 2 (n)
    9. n * 2^n
    10. ln(n)
    11. e^n
    12. n! (factorial) - capped at 20 to avoid overflow
*/

// 1. n (identity)
int nValue(int n) {
    return n;
}

// 2. (3/2)^n
long double threeByTwo(int n) {
    return powl(3.0L/2.0L, (long double)n);
}

// 3. n^3
long long nCube(int n) {
    return (long long)n * n * n;
}

// 4. (log base 2 (n))
long double log2N(int n) {
    if (n <= 0) return 0.0L;
    return log2l((long double)n);
}

// 5. log base 2 (n!) computed as sum_{i=1..n} log2(i)
long double factorial(int n) {
    if (n <= 1) return 0.0L;
    int limit = n;
    long double logSum = 0.0L;
    for (int i = 2; i <= n; ++i) {
        logSum += log2l((long double)i);
    }
    return logSum;
}

long double log2OfFactorial(int n) {
    return factorial(n);
}

// 6. 2^(2n)
long double twoPow2n(int n) {
    return powl(2.0L, 2.0L * (long double)n);
}

// 7. log(log(n)) (natural logs)
long double logLogN(int n) {
    if (n <= 1) return 0.0L;
    return logl(logl((long double)n));
}

// 8. log base 2 (n) wrapper (already log2N)

// 9. n * 2^n
long double nTimes2PowN(int n) {
    return (long double)n * powl(2.0L, (long double)n);
}

// 10. natural log ln(n)
long double logN(int n) {
    if (n <= 0) return 0.0L;
    return logl((long double)n);
}

// 11. e^n
long double expN(int n) {
    return expl((long double)n);
}

long long factorialValue(int n) {
    if (n < 0) return 0; // undefined for negative
    if (n == 0 || n == 1) return 1;
    long long result = 1;
    for (int i = 2; i <= n && i <= 20; ++i) { // cap at 20 to avoid overflow
        result *= i;
    }
    return result;
}

int main() {
    FILE *f = fopen("output.csv", "w");
    if (!f) {
        perror("fopen");
        return 1;
    }
    fprintf(f, "n,(3/2)^n,n^3,(lg n)^2,lg(n!),2^(2n),log(log n),lg n,n*2^n,ln n,e^n,n!\n");
    for (int i = 1; i <= 100; ++i) {
        int n = i;
        long double v_threeByTwo = threeByTwo(i);
        long long v_nCube = nCube(i);
        long double v_log2n = log2N(i);
        long double v_log2n_sq = (v_log2n == 0.0L) ? 0.0L : v_log2n * v_log2n;
        long double v_log2_fact = log2OfFactorial(i);
        long double v_twoPow2n = twoPow2n(i);
        long double v_loglogn = logLogN(i);
        long double v_lg = v_log2n;
        long double v_nTimes2n = nTimes2PowN(i);
        long double v_lnn = logN(i);
        long double v_expn = expN(i);
        long long v_fact = factorialValue(i);

        fprintf(f, "%d,%.10Lf,%lld,%.10Lf,%.10Lf,%.10Lf,%.10Lf,%.10Lf,%.10Lf,%.10Lf,%.10Lf,%lld\n",
                n, v_threeByTwo, v_nCube, v_log2n_sq, v_log2_fact, v_twoPow2n, v_loglogn, v_lg, v_nTimes2n, v_lnn, v_expn, v_fact);
    }
    fclose(f);
    printf("Wrote output to output.csv\n");
    return 0;
}