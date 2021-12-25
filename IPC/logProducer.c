#include "logheader.h"
#define BUFFSIZE 200

int main (int argc, char **argv)
{
    key_t s_key;


    int shm_id;
    struct shared_memory *shared_mem_ptr;
    int mutex_sem, buffer_count_sem, logger_signal_sem;
    

    // shared_mem_ptr = initialize_producer(&s_key,&mutex_sem,&buffer_count_sem,&logger_signal_sem,&shm_id,shared_mem_ptr);
    
    int faield
    char * myfifo = '/tmp/myfifo';

    mkfifo(myfifo,0666);

    
    struct sembuf asem [1];

    asem [0].sem_num = 0;
    asem [0].sem_op = 0;
    asem [0].sem_flg = 0;

    char buf [BUFFSIZE];

    printf ("type to produce log: ");

    while (fgets (buf, BUFFSIZE-2, stdin)) {
        // log preprocessing
        int length = strlen (buf);
        log_preprocess(buf,length);

        fd = open(myfifo, O_WRONLY);
        write(fd,"%d:[%s]\n", getpid (), buf);
        close(fd);

        printf ("type to produce log: ");
    }
 
    unlink(myfifo);
    exit (0);
}

