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

int main()
{
    int sem = semget(SEM_KEY, 1, 0);

    if (sem == -1)
    {
        printf("%s\n", strerror(errno));
        return 0;
    }

    struct sembuf sb;
    sb.sem_flg = SEM_UNDO;
    sb.sem_num = 0;
    sb.sem_op = -1;

    printf("Attempting to access shared data\n");
    semop(sem, &sb, 1);
    printf("Successfully accessed\n\n");

    int out = open(FILE_NAME, O_WRONLY | O_APPEND);
    int shm = shmget(SHM_KEY, 0, 0);

    char *data = shmat(shm, 0, 0);

    printf("Previous line: %s\n", data);
    printf("Current line: ");

    char *input;
    fgets(input, 2998, stdin);

    if (strlen(input) > 0 && input[strlen(input) - 1] != '\n')
    {
        input[strlen(input) + 1] = '\0';
        input[strlen(input)] = '\n';
    }

    strcpy(data, input);
    write(out, data, strlen(data) * sizeof(char));

    shmdt(data);

    sb.sem_op = 1;

    semop(sem, &sb, 1);

    printf("Shared resources closed or returned to previous state\n");

    return 0;
}