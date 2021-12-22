#include "logheader.h"



void error (char *msg);

int main (int argc, char **argv)
{

    FILE *fp;
    fp = fopen("log.txt","w+")
    



    key_t s_key;

    union semun  
    {
        int val;
        struct semid_ds *buf;
        ushort array [1];
    } sem_attr;

    int shm_id;
    struct shared_memory *shared_mem_ptr;
    int mutex_sem, buffer_count_sem, spool_signal_sem;
    
    printf ("logger: hello world\n");


    if ((s_key = ftok (SEM_MUTEX_KEY, PROJECT_ID)) == -1)
        error ("ftok");

    if ((mutex_sem = semget (s_key, 1, 0660 | IPC_CREAT)) == -1)
        error ("semget");

    sem_attr.val = 0;        
    if (semctl (mutex_sem, 0, SETVAL, sem_attr) == -1)
        error ("semctl SETVAL");
    

    if ((s_key = ftok (SHARED_MEMORY_KEY, PROJECT_ID)) == -1)
        error ("ftok");    
    if ((shm_id = shmget (s_key, sizeof (struct shared_memory), 
         0660 | IPC_CREAT)) == -1)
        error ("shmget");
    if ((shared_mem_ptr = (struct shared_memory *) shmat (shm_id, NULL, 0)) 
         == (struct shared_memory *) -1) 
        error ("shmat");

    shared_mem_ptr -> buffer_index = shared_mem_ptr -> buffer_print_index = 0;


    if ((s_key = ftok (SEM_BUFFER_COUNT_KEY, PROJECT_ID)) == -1)
        error ("ftok");
    if ((buffer_count_sem = semget (s_key, 1, 0660 | IPC_CREAT)) == -1)
        error ("semget");

    sem_attr.val = MAX_BUFFERS;    
    if (semctl (buffer_count_sem, 0, SETVAL, sem_attr) == -1)
        error ("semctl SETVAL");


    if ((s_key = ftok (SEM_SPOOL_SIGNAL_KEY, PROJECT_ID)) == -1)
        error ("ftok");
    if ((spool_signal_sem = semget (s_key, 1, 0660 | IPC_CREAT)) == -1)
        error ("semget");

    sem_attr.val = 0;    
    if (semctl (spool_signal_sem, 0, SETVAL, sem_attr) == -1)
        error ("semctl SETVAL");
    

    sem_attr.val = 1;
    if (semctl (mutex_sem, 0, SETVAL, sem_attr) == -1)
        error ("semctl SETVAL"); 

    struct sembuf asem [1];

    asem [0].sem_num = 0;
    asem [0].sem_op = 0;
    asem [0].sem_flg = 0;

    while (1) {  

        asem [0].sem_op = -1;
        if (semop (spool_signal_sem, asem, 1) == -1)
	    perror ("semop: spool_signal_sem");
    
        fprintf (fp,"%s", shared_mem_ptr -> buf [shared_mem_ptr -> buffer_print_index]);


        (shared_mem_ptr -> buffer_print_index)++;
        if (shared_mem_ptr -> buffer_print_index == MAX_BUFFERS)
           shared_mem_ptr -> buffer_print_index = 0;


        asem [0].sem_op = 1;
        if (semop (buffer_count_sem, asem, 1) == -1)
	    perror ("semop: buffer_count_sem");
    }
    fclose(fp);
}

//TODO: close open file
void error (char *msg)
{
    perror (msg);
    exit (1);
}