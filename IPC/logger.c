#include "logheader.h"



int main (int argc, char **argv)
{

    key_t s_key;

    semun sem_attr;

    int shm_id;
    struct shared_memory *shared_mem_ptr;
    int mutex_sem, buffer_count_sem, logger_signal_sem;
    
    printf ("logger: start initialization\n");

    shared_mem_ptr = initialize(&s_key,&mutex_sem,&buffer_count_sem,&logger_signal_sem,&sem_attr,&shm_id,shared_mem_ptr);

    printf ("logger: complete initialization\n");

    printf ("logger: ready\n");

    struct sembuf mysem [1];

    mysem [0].sem_num = 0;
    mysem [0].sem_op = 0;
    mysem [0].sem_flg = 0;

    while (1) {  

        mysem [0].sem_op = -1;
        if (semop (logger_signal_sem, mysem, 1) == -1)
	        perror ("semop: logger_signal_sem");
    
        FILE *fp;
        fp = fopen("log.txt","a+");
        fprintf (fp,"%s", shared_mem_ptr -> buf [shared_mem_ptr -> buffer_print_index]);
        fclose(fp);

        (shared_mem_ptr -> buffer_print_index)++;
        if (shared_mem_ptr -> buffer_print_index == MAX_BN)
           shared_mem_ptr -> buffer_print_index = 0;


        mysem [0].sem_op = 1;
        if (semop (buffer_count_sem, mysem, 1) == -1)
	    perror ("semop: buffer_count_sem");
    }
    
}

