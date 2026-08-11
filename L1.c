#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define MIN_SIZE 1000

void merge(int arr[], int left, int mid, int right)
{
    int i, j, k;

    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    if (L == NULL || R == NULL)
    {
        printf("Memory allocation failed.\n");
        free(L);
        free(R);
        exit(1);
    }

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSortSeq(int arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSortSeq(arr, left, mid);
        mergeSortSeq(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void mergeSortParallel(int arr[], int left, int right)
{
    if (left < right)
    {
        /* Use sequential sorting for small arrays */
        if ((right - left + 1) <= MIN_SIZE)
        {
            mergeSortSeq(arr, left, right);
            return;
        }

        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergeSortParallel(arr, left, mid);
            }

            #pragma omp section
            {
                mergeSortParallel(arr, mid + 1, right);
            }
        }

        merge(arr, left, mid, right);
    }
}

int main()
{
    int n = 100000;

    int *arr_seq = (int *)malloc(n * sizeof(int));
    int *arr_par = (int *)malloc(n * sizeof(int));

    if (arr_seq == NULL || arr_par == NULL)
    {
        printf("Memory allocation failed.\n");
        free(arr_seq);
        free(arr_par);
        return 1;
    }

    /* Generate random numbers */
    srand((unsigned int)time(NULL));

    for (int i = 0; i < n; i++)
    {
        arr_seq[i] = rand() % 10000;
        arr_par[i] = arr_seq[i];
    }

    printf("Sorting %d elements...\n\n", n);

    /* Sequential merge sort */
    double start_time = omp_get_wtime();

    mergeSortSeq(arr_seq, 0, n - 1);

    double time_seq = omp_get_wtime() - start_time;

    printf("Sequential Merge Sort Time: %f seconds\n", time_seq);

    /* Parallel merge sort */
    start_time = omp_get_wtime();

    mergeSortParallel(arr_par, 0, n - 1);

    double time_par = omp_get_wtime() - start_time;

    printf("Parallel Merge Sort Time:   %f seconds\n", time_par);

    printf("\nDifference (Sequential - Parallel): %f seconds\n",
           time_seq - time_par);

    if (time_par > 0)
    {
        printf("Speedup: %.2fx\n", time_seq / time_par);
    }

    /* Verify that the parallel array is sorted */
    int sorted = 1;

    for (int i = 0; i < n - 1; i++)
    {
        if (arr_par[i] > arr_par[i + 1])
        {
            sorted = 0;
            break;
        }
    }

    if (sorted)
        printf("\nVerification: Array is sorted correctly.\n");
    else
        printf("\nVerification: Sorting failed.\n");

    /* Print first 20 elements */
    printf("\nFirst 20 elements of the sorted array:\n");

    for (int i = 0; i < (n < 20 ? n : 20); i++)
    {
        printf("%d ", arr_par[i]);
    }

    printf("\n");

    free(arr_seq);
    free(arr_par);

    return 0;
}