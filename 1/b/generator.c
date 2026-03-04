#include <stdio.h>
#include <stdlib.h>

int main() {
    int N = 100000;
    FILE* fptr = fopen("random100000.csv", "a");
    for(int i = 0; i < N; i++) {
        int value = rand() % (N+1);
        fprintf(fptr, "%d\n", value);
    }
    fclose(fptr);
    return 0;
}
