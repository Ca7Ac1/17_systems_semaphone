#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "constants.h"

int main(int argc, char *argv[])
{
    char *option;

    if (argc > 1 && strcmp(argv[1], "remove") == 0)
    {
        int shm = shmget(SHM_KEY, 0, IPC_CREAT);
        shmctl(shm, IPC_RMID, 0);

        int sem = semget(SEM_KEY, 1, IPC_CREAT);
        semctl(sem, IPC_RMID, 0);
    }
    else if (argc > 1 && strcmp(argv[1], "create") == 0)
    {
        open(FILE_NAME, O_CREAT | O_TRUNC, 0644);

        shmget(SHM_KEY, sizeof(char) * 3000, IPC_CREAT | IPC_EXCL | 0640);

        int sem = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL);
        union semun set;
        set.val = 1;
        semctl(sem, 0, SETVAL, set);
    }
    else
    {
        printf("Incorrect or no command line argument given:\nGive \"remove\" for removal of shared resources\nGive \"create\" for creation of shared resources\n");
    }

    return 0;
}
