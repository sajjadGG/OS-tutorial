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


void error (char *msg);
void log_preprocess(char buf[], int length);

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

struct shared_memory * initialize(key_t * s_key,int *ms, int *bcs, int *sss, semun * sem_attr, int* shm, struct shared_memory *shared_mem_ptr);

struct shared_memory * initialize_producer(key_t * s_key,int *ms, int *bcs, int *sss, int* shm, struct shared_memory *shared_mem_ptr);

