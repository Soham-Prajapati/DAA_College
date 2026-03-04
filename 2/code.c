#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    // Copy data to temp arrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Merge temp arrays back
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // Free memory
    free(L);
    free(R);
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];   // choose last element as pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
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
    FILE *outptr = fopen("runningTimes.csv", "a");
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
        mergeSort(arr1, 0, i-1);
        clock_t end1 = clock();

        double time1 = (double)(end1 - start1) / CLOCKS_PER_SEC;

        clock_t start2 = clock();
        quickSort(arr2, 0, i-1);
        clock_t end2 = clock();

        double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;

        fprintf(outptr,"%d,%lf,%lf\n", i, time1, time2);
    }
    fclose(outptr);
    return 0;
}