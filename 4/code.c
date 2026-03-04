#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to generate random array and save to CSV
void generate_random_csv(const char *filename, int size) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        int num = (rand() % 201) - 100; // Range: -100 to 100
        fprintf(fp, "%d", num);
        if (i != size - 1) fprintf(fp, ",");
    }
    fprintf(fp, "\n");
    fclose(fp);
}

// Function to read array from CSV
int read_csv(const char *filename, int *arr, int max_size) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    int i = 0;
    while (fscanf(fp, "%d,", &arr[i]) == 1 && i < max_size) i++;
    fclose(fp);
    return i;
}

int main() {
    int max_size = 1000;
    int arr[max_size];
    char csv_file[] = "generated.csv";
    FILE *out_fp = fopen("output.csv", "w");
    if (!out_fp) {
        printf("Error opening output.csv!\n");
        return 1;
    }
    fprintf(out_fp, "ArraySize,TimeTaken(ms)\n");
    for (int size = 100; size <= max_size; size += 100) {
        generate_random_csv(csv_file, size);
        int n = read_csv(csv_file, arr, size);
        clock_t start_time = clock();
        // Kadane's algorithm
        int max_sum = arr[0], cur_sum = arr[0];
        int start = 0, end = 0, temp_start = 0;
        for (int i = 1; i < n; i++) {
            if (cur_sum < 0) {
                cur_sum = arr[i];
                temp_start = i;
            } else {
                cur_sum += arr[i];
            }
            if (cur_sum > max_sum) {
                max_sum = cur_sum;
                start = temp_start;
                end = i;
            }
        }
        clock_t end_time = clock();
        double time_taken = ((double)(end_time - start_time) * 1000.0) / CLOCKS_PER_SEC;
        fprintf(out_fp, "%d,%.3f\n", size, time_taken);
        printf("Array size: %d\n", size);
        printf("Maximum subarray sum: %d\n", max_sum);
        printf("Subarray indices: i = %d, j = %d\n", start, end);
        printf("Subarray: ");
        for (int k = start; k <= end; k++) {
            printf("%d ", arr[k]);
        }
        printf("\n\n");
    }
    fclose(out_fp);
    return 0;
}