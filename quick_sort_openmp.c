#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

#define LENGTH      1000000   
#define NUM_THREADS 2         
#define THRESHOLD  LENGTH/10000   
       

// Generate random array
void fill_random_array(int *array, int n, int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 1000000;  
    }
}

// partition for quicksort 
int partitionArray(int *a, int left, int right) {
    int pivot = a[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (a[j] <= pivot) {
            i++;
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }
    int temp = a[i + 1];
    a[i + 1] = a[right];
    a[right] = temp;
    return i + 1;
}

// main parallel quicksort
void Quicksort(int *array, int left, int right) {
    if (left < right) {
        int pivot = partitionArray(array, left, right);
        #pragma omp task firstprivate(array, left, pivot) if((pivot - left) > THRESHOLD) final((pivot - left) < THRESHOLD) 
        {
            Quicksort(array, left, pivot - 1);
        }
        #pragma omp task firstprivate(array, right, pivot) if((right - pivot) > THRESHOLD) final((right - pivot) < THRESHOLD)
        {
            Quicksort(array, pivot + 1, right);
        }
        #pragma omp taskwait
    }
}


int is_sorted(int *a, int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i])
            return 0;
    }
    return 1;
}

int main() {
    int n = LENGTH;
    int seed = 100;  

    int *array = (int *)malloc(n * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(0);
    }


    fill_random_array(array, n, seed);
    omp_set_num_threads(NUM_THREADS);
    clock_t begin = clock();

    #pragma omp parallel default(none) shared(array, n)
    {
        #pragma omp single
        {
            Quicksort(array, 0, n - 1);
        }
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time taken for parallel quicksort: %f seconds\n", time_spent);


    if (is_sorted(array, n))
        printf("Array is sorted in ascending order.\n");
    else
        printf("Array is NOT sorted\n");

    free(array);
    return 0;
}
