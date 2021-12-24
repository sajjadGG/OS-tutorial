#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/sem.h>


#define MAX_BN 10
#define SMK_PATH "logger.c"
#define SEMK_PATH "logProducer.c"
#define SEMBC_PATH "logheader.h"
#define SEMSS_PATH "Makefile"
#define PROJECT_ID 'S'

struct shared_memory {
    char buf [MAX_BN] [256];
    int buffer_index;
    int buffer_print_index;
};

typedef union   
{
    int val;
    struct semid_ds *buf;
    ushort array [1];
}semun;


