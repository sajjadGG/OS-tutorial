#include "logheader.h"


void error (char *msg);

void initialize(key_t * s_key,int *ms, int *bcs, int *sss, semun * sem_attr, int* shm, struct shared_memory *shared_mem_ptr)
{
    if ((*s_key = ftok (SEMK_PATH, PROJECT_ID)) == -1)
        error ("ftok failed for SMK");

    if ((*ms = semget (*s_key, 1, 0660 | IPC_CREAT)) == -1)
        error ("semget failed for SEMK");

    sem_attr->val = 0;        
    if (semctl (*ms, 0, SETVAL, *sem_attr) == -1)
        error ("semctl SETVAL");

    if ((*s_key = ftok (SMK_PATH, PROJECT_ID)) == -1)
        error ("ftok failed form SMK");

    if ((*shm = shmget (*s_key, sizeof (struct shared_memory), 
         0660 | IPC_CREAT)) == -1)
        error ("shmget failed for SMK"); 

    if ((shared_mem_ptr = (struct shared_memory *) shmat (*shm, NULL, 0)) 
         == (struct shared_memory *) -1) 
        error ("shmat");

    shared_mem_ptr -> buffer_index = shared_mem_ptr -> buffer_print_index = 0;

    if ((*s_key = ftok (SEMBC_PATH, PROJECT_ID)) == -1)
        error ("ftok faile for SEMBC");

    if ((*bcs = semget (*s_key, 1, 0660 | IPC_CREAT)) == -1)
        error ("semget faile for SEMBC");

    sem_attr->val = MAX_BN; 

    if (semctl (*bcs, 0, SETVAL, *sem_attr) == -1)
        error ("semctl SETVAL");

    if ((*s_key = ftok (SEMSS_PATH, PROJECT_ID)) == -1)
        error ("ftok failed for SEMSS");
    if ((*sss = semget (*s_key, 1, 0660 | IPC_CREAT)) == -1)
        error ("semget faield for SEMSS");

    
    sem_attr->val = 0;    
    if (semctl (*sss, 0, SETVAL, *sem_attr) == -1)
        error ("semctl SETVAL failed for SSS");
    

    sem_attr->val = 1;
    if (semctl (*ms, 0, SETVAL, *sem_attr) == -1)
        error ("semctl SETVAL failed for SSS"); 

}


int main (int argc, char **argv)
{

    key_t s_key;

    semun sem_attr;

    int shm_id;
    struct shared_memory *shared_mem_ptr;
    int mutex_sem, buffer_count_sem, spool_signal_sem;
    
    printf ("logger: hello world\n");

    initialize(&s_key,&mutex_sem,&buffer_count_sem,&spool_signal_sem,&sem_attr,&shm_id,shared_mem_ptr);


    struct sembuf mysem [1];

    mysem [0].sem_num = 0;
    mysem [0].sem_op = 0;
    mysem [0].sem_flg = 0;

    while (1) {  

        mysem [0].sem_op = -1;
        if (semop (spool_signal_sem, mysem, 1) == -1)
	        perror ("semop: spool_signal_sem");
    
        FILE *fp;
        fp = fopen("log.txt","w+");
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

//TODO: close open file
void error (char *msg)
{
    perror (msg);
    exit (1);
}