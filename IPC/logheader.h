#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_BUF 1024

#define MAX_BN 10
#define MKPIPE "/tmp/myfifo"
#define MKRES "log.txt"

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

int * get_number(long* pos);