#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/sem.h>


#define MAX_BN 10
#define SMK_PATH "/tmp/shared_memory_key"
#define SEMK_PATH "/tmp/sem-mutex-key"
#define SEMBC_PATH "/tmp/sem-buffer-count-key"
#define SEMSS_PATH "/tmp/sem-spool-signal-key"
#define PROJECT_ID 'K'

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


