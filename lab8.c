#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size;
    int data[4] = {10, 20, 30, 40};
    int recv;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* This program requires exactly 4 processes */
    if (size != 4) {
        if (rank == 0)
            printf("Please run the program with exactly 4 processes.\n");

        MPI_Finalize();
        return 0;
    }

    /* Scatter one element to each process */
    MPI_Scatter(data, 1, MPI_INT,
                &recv, 1, MPI_INT,
                0, MPI_COMM_WORLD);

    /* Modify the received value */
    recv += 1;

    /* Gather the modified values at process 0 */
    MPI_Gather(&recv, 1, MPI_INT,
               data, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    /* Print the result */
    if (rank == 0) {
        printf("Gathered data: ");

        for (int i = 0; i < size; i++)
            printf("%d ", data[i]);

        printf("\n");
    }

    MPI_Finalize();

    return 0;
}