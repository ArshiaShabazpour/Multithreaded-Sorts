#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>

#define LENGTH      1000000
#define DEPTH 8
#define THRESHOLD   (LENGTH / 10000)

void fill_random_array(int *array, int n, int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 1000000;  
    }
}

typedef struct quickSort_parameters {
    int *array;
    int left;
    int right;
    int depthOfThreadCreation;
} quickSort_parameters;

int partition(int *array, int left, int right) {
    int pivot = array[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (array[j] <= pivot) {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    int temp = array[i + 1];
    array[i + 1] = array[right];
    array[right] = temp;
    return i + 1;
}

void quickSort(int *array, int left, int right) {
    if (left < right) {
        int pivotPos = partition(array, left, right);
        quickSort(array, left, pivotPos - 1);
        quickSort(array, pivotPos + 1, right);
    }
}

void concurrent_quickSort(int *array, int left, int right, int depthOfThreadCreation);


void* worker_quickSort(void * arg) {
    quickSort_parameters *params = (quickSort_parameters *)arg;
    concurrent_quickSort(params->array, params->left, params->right, params->depthOfThreadCreation);
}


void concurrent_quickSort(int *array, int left, int right, int depthOfThreadCreation) {
    if (left < right) {
        int pivotPos = partition(array, left, right);
        if (depthOfThreadCreation > 0 && (right - left + 1) >= THRESHOLD) {
            quickSort_parameters *params = (quickSort_parameters *)malloc(sizeof(quickSort_parameters));
            if (params == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }        
            params->left = left;
            params->right = pivotPos - 1;
            params->depthOfThreadCreation = depthOfThreadCreation - 1;
            params->array = array;
            pthread_t thread;
            if (pthread_create(&thread, NULL, worker_quickSort, params) != 0) {
                perror("Thread Creation Error\n");
                free(params);
                exit(EXIT_FAILURE);
            }
            concurrent_quickSort(array, pivotPos + 1, right, depthOfThreadCreation - 1);
            pthread_join(thread, NULL);
        } else {
            quickSort(array, left, pivotPos - 1);
            quickSort(array, pivotPos + 1, right);
        }
    }
}

int main(void) {
    int size = LENGTH;
    int depthOfThreadCreation = DEPTH;
    
    int *arrayElements = (int *)malloc(size * sizeof(int));
    if (arrayElements == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    fill_random_array(arrayElements, size, 100);  
    
    double begin = clock();

    concurrent_quickSort(arrayElements, 0, size - 1, depthOfThreadCreation);

    double end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("The execution time was: %lf secs\n", time_spent);
    free(arrayElements);
    return 0;
}