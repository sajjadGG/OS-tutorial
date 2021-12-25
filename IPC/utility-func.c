#include "logheader.h"

void log_preprocess(char buf[], int length){
    if (buf [length - 1] == '\n')
           buf [length - 1] = '\0';
}


struct shared_memory * initialize(key_t * s_key,int *ms, int *bcs, int *sss, semun * sem_attr, int* shm, struct shared_memory *shared_mem_ptr)
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

    return shared_mem_ptr;

}



struct shared_memory * initialize_producer(key_t * s_key,int *ms, int *bcs, int *sss, int* shm, struct shared_memory *shared_mem_ptr)
{
    if ((*s_key = ftok (SEMK_PATH, PROJECT_ID)) == -1)
        error ("ftok failed for SMK");

    if ((*ms = semget (*s_key, 1, 0)) == -1)
        error ("semget failed for SEMK");


    if ((*s_key = ftok (SMK_PATH, PROJECT_ID)) == -1)
        error ("ftok failed form SMK");

    if ((*shm = shmget (*s_key, sizeof (struct shared_memory), 0)) == -1)
        error ("shmget failed for SMK"); 

    if ((shared_mem_ptr = (struct shared_memory *) shmat (*shm, NULL, 0)) 
         == (struct shared_memory *) -1) 
        error ("shmat");


    if ((*s_key = ftok (SEMBC_PATH, PROJECT_ID)) == -1)
        error ("ftok faile for SEMBC");

    if ((*bcs = semget (*s_key, 1, 0)) == -1)
        error ("semget faile for SEMBC");



    if ((*s_key = ftok (SEMSS_PATH, PROJECT_ID)) == -1)
        error ("ftok failed for SEMSS");
    if ((*sss = semget (*s_key, 1, 0)) == -1)
        error ("semget faield for SEMSS");

    return shared_mem_ptr;

}

//TODO: close open file
void error (char *msg)
{
    perror (msg);
    exit (1);
}

