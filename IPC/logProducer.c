#include "logheader.h"
#define BUFFSIZE 200

int main (int argc, char **argv)
{

    FILE* fd;
    char * myfifo = MKPIPE;

    mkfifo(myfifo,0666);

    
    struct sembuf asem [1];

    asem [0].sem_num = 0;
    asem [0].sem_op = 0;
    asem [0].sem_flg = 0;

    char buf [BUFFSIZE];
    char filebuf[2*BUFFSIZE]; // for preprocessing purpose (headroom)

    printf ("type to produce log: ");

    while (fgets (buf, BUFFSIZE-2, stdin)) {
        // log preprocessing
        int length = strlen (buf);
        log_preprocess(buf,length);

        fd = fopen(myfifo, "a+");
        fprintf(fd,"%d:[%s]\n", getpid (), buf);
        fclose(fd);

        printf ("type to produce log: ");
    }
 
    unlink(myfifo);
    exit (0);
}

