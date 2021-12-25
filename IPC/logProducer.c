#include "logheader.h"


void error (char *msg);

int main (int argc, char **argv)
{
    key_t s_key;


    int shm_id;
    struct shared_memory *shared_mem_ptr;
    int mutex_sem, buffer_count_sem, logger_signal_sem;
    

    initialize_producer(&s_key,&mutex_sem,&buffer_count_sem,&logger_signal_sem,&shm_id,shared_mem_ptr);
    struct sembuf asem [1];

    asem [0].sem_num = 0;
    asem [0].sem_op = 0;
    asem [0].sem_flg = 0;

    char buf [200];

    printf ("Please type a message: ");

    while (fgets (buf, 198, stdin)) {
        // remove newline from string
        int length = strlen (buf);
        if (buf [length - 1] == '\n')
           buf [length - 1] = '\0';


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

        printf ("Please type a message: ");
    }
 
    if (shmdt ((void *) shared_mem_ptr) == -1)
        error ("shmdt");
    exit (0);
}

// Print system error and exit
void error (char *msg)
{
    perror (msg);
    exit (1);
}