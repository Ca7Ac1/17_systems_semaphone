#ifndef CONSTANTS_H
#define CONSTANTS_H

union semun
{
    int val;               // used for SETVAL
    struct semid_ds *buf;  // used for IPC_STAT and IPC_SET
    unsigned short *array; // used for SETALL
    struct seminfo *__buf;
};

const char *FILE_NAME = "telephone.txt";
const int SHM_KEY = 101;
const int SEM_KEY = 102;

#endif