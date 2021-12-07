#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

#include "constants.h"

int main(int argc, char *argv[])
{
    char *option;

    if (argc > 1 && strcmp(argv[1], "-r") == 0)
    {
        open(FILE_NAME, O_WRONLY | O_TRUNC);

        int shm = shmget(SHM_KEY, 1, 0);
        shmctl(shm, IPC_RMID, 0);

        int sem = semget(SEM_KEY, 1, 0);
        semctl(sem, IPC_RMID, 0);

        printf("Shared resources closed\n");
    }
    else if (argc > 1 && strcmp(argv[1], "-c") == 0)
    {
        open(FILE_NAME, O_CREAT | O_TRUNC | O_EXCL, 0644);
        printf("File created\n");

        shmget(SHM_KEY, sizeof(char) * 3000, IPC_CREAT | IPC_EXCL | 0640);
        printf("Shared memory created\n");

        int sem = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
        union semun set;
        set.val = 1;
        semctl(sem, 0, SETVAL, set);
        printf("Semaphore created\n");
    }
    else if (argc > 1 && strcmp(argv[1], "-v") == 0)
    {
        int f = open(FILE_NAME, O_CREAT | O_RDONLY, 0644);
        struct stat *size;
        stat(FILE_NAME, size);

        char *telephone = malloc(size->st_size);
        read(f, telephone, size->st_size);

        printf("%s\n", telephone);

        free(telephone);
    }
    else
    {
        printf("Incorrect or no command line argument given:\nGive \"-r\" for removal of shared resources\nGive \"-c\" for creation of shared resources\nGive \"-v\" for viewing the telephone file so far\n");
    }

    return 0;
}
