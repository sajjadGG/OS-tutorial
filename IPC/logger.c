#include "logheader.h"



int main (int argc, char **argv)
{



    printf ("logger: start initialization\n");

    // shared_mem_ptr = initialize(&s_key,&mutex_sem,&buffer_count_sem,&logger_signal_sem,&sem_attr,&shm_id,shared_mem_ptr);

    printf ("logger: complete initialization\n");

    printf ("logger: ready\n");

    while (1) {  

        fd = open(myfifo, O_RDONLY);
        read(fd, buf, MAX_BUF);
    
        FILE *fp;
        fp = fopen("log.txt","a+");
        fprintf (fp,"%s", buf);
        fclose(fp);
    }
    
}

