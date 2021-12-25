#include "logheader.h"



int main (int argc, char **argv)
{


    
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buf[MAX_BUF];
    long pos=0;

    printf ("logger: start initialization\n");

    // shared_mem_ptr = initialize(&s_key,&mutex_sem,&buffer_count_sem,&logger_signal_sem,&sem_attr,&shm_id,shared_mem_ptr);

    printf ("logger: complete initialization\n");

    printf ("logger: ready\n");

    while (1) {  

        get_number(&pos);


    }
    
}

