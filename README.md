<p align="center">
  <img src="https://lh6.googleusercontent.com/proxy/7YK4zsRP98qN1TVP594RxgaSpdRmjHASCkwuYQ2z5AuglpU7oq-wD0AWIBPlSjrsJChcSdYEv04z7yiZdUkSkLiuDqjm34TFPxnK-QCy_sDZmHg" alt="Pthreads Logo" height="100">
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/e/eb/OpenMP_logo.png/1200px-OpenMP_logo.png" alt="OpenMP Logo" height="100">
</p>

# Parallel Sorting Algorithms with OpenMP and Pthreads

This project demonstrates parallel implementations of **Merge Sort** and **Quick Sort** using **OpenMP** and **Pthreads**. The goal is to compare the ease, efficiency, and structure of multithreaded sorting algorithms across different parallel programming paradigms.

## 📂 Files Included

| Filename                | Algorithm   | Parallelization | Description |
|------------------------|-------------|------------------|-------------|
| `mergeOpenmp.c`        | Merge Sort  | OpenMP           | Divides the array into segments and sorts them in parallel using OpenMP sections, then merges the results. |
| `mergePthread.c`       | Merge Sort  | Pthreads         | Uses multiple threads to sort separate parts of the array and merges them afterward. |
| `quick_sort_openmp.c`  | Quick Sort  | OpenMP           | Parallelizes recursive quicksort tasks with OpenMP's tasking model. |
| `quick_sort_pthread.c` | Quick Sort  | Pthreads         | Implements a threaded version of quicksort with depth control to avoid thread explosion. |

---

## ⚙️ Compilation

### OpenMP Versions

```bash
gcc -fopenmp -o mergeOpenmp mergeOpenmp.c
gcc -fopenmp -o quick_sort_openmp quick_sort_openmp.c
```

### Pthreads Versions

```bash
gcc -pthread -o mergePthread mergePthread.c
gcc -pthread -o quick_sort_pthread quick_sort_pthread.c
```

---

## ▶️ Execution

Each executable will:

- Generate a large array of 1,000,000 random integers
- Sort the array using the specified algorithm and method
- Print time taken and verify if the array is sorted

```bash
./mergeOpenmp
./mergePthread
./quick_sort_openmp
./quick_sort_pthread
```

---

## 📝 Notes

- **Thread Count**: Defined by `NUM_THREADS` in source files (default: 2).
- **Array Size**: 1,000,000 elements.
- **Verification**: Each program checks if the resulting array is sorted.
- **Performance**: Will vary based on CPU cores and load.

---
