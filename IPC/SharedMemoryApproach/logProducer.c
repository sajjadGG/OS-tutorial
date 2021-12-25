#include "logheader.h"
#define BUFFSIZE 200

int main (int argc, char **argv)
{
    key_t s_key;


    int shm_id;
    struct shared_memory *shared_mem_ptr;
    int mutex_sem, buffer_count_sem, logger_signal_sem;
    

    shared_mem_ptr = initialize_producer(&s_key,&mutex_sem,&buffer_count_sem,&logger_signal_sem,&shm_id,shared_mem_ptr);
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



        asem [0].sem_op = -1;
        if (semop (buffer_count_sem, asem, 1) == -1)
	        error ("semop: buffer_count_sem");
    

        asem [0].sem_op = -1;
        if (semop (mutex_sem, asem, 1) == -1)
	        error ("semop: mutex_sem");


        sprintf (shared_mem_ptr -> buf [shared_mem_ptr -> buffer_index], "%d:[%s]\n", getpid (), buf);
            (shared_mem_ptr -> buffer_index)++;
            if (shared_mem_ptr -> buffer_index == MAX_BN)
                shared_mem_ptr -> buffer_index = 0;

        asem [0].sem_op = 1;
        if (semop (mutex_sem, asem, 1) == -1)
	    error ("semop: mutex_sem");
    

        asem [0].sem_op = 1;
        if (semop (logger_signal_sem, asem, 1) == -1)
	    error ("semop: logger_signal_sem");

        printf ("type to produce log: ");
    }
 
    if (shmdt ((void *) shared_mem_ptr) == -1)
        error ("shmdt");
    exit (0);
}

