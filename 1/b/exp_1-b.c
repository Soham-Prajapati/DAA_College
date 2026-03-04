#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

void copyArray(int src[], int dest[], int n){
    for(int i=0; i<n; i++){
        dest[i] = src[i];
    }
}

int main(void){
    FILE *fptr = fopen("random100000.csv", "r");
    if(fptr == NULL){
        printf("Error opening file.\n");
        return 1;
    }
    int originalArray[100000];
    for(int i=0; i<100000; i++){
        fscanf(fptr, "%d", &originalArray[i]);
    }
    fclose(fptr);

    int N = 1000;
    int m = 100;
    FILE *outptr = fopen("runningTimes.csv", "w");
    if(outptr == NULL){
        printf("Error opening output file.\n");
        return 1;
    }
    for(int i=m; i<=(N*m); i+=m){
        int arr1[100000];
        int arr2[100000];
        copyArray(originalArray, arr1, i);
        copyArray(originalArray, arr2, i);
        
        clock_t start1 = clock();
        insertionSort(arr1, i);
        clock_t end1 = clock();

        double time1 = (double)(end1 - start1) / CLOCKS_PER_SEC;

        clock_t start2 = clock();
        selectionSort(arr2, i);
        clock_t end2 = clock();

        double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;

        fprintf(outptr,"%d,%lf,%lf\n", i, time1, time2);
    }
    fclose(outptr);
    return 0;
}