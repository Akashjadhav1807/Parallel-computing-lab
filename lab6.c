#include <stdio.h>
#include <mpi.h>

#define SIZE 200000000  

int main(int argc, char *argv[]) {
    int rank;
    static int data_send[SIZE];
    static int data_recv[SIZE];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    data_send[0] = rank;

    if (rank == 0) {
        MPI_Send(data_send, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(data_recv, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == 1) {
        MPI_Send(data_send, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(data_recv, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    printf("Process %d received %d\n", rank, data_recv[0]);

    MPI_Finalize();
    return 0;
}