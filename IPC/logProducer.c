#include "logheader.h"
#define BUFFSIZE 200

int main (int argc, char **argv)
{

    int fd;
    char * myfifo = '/tmp/myfifo';

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

        fd = open(myfifo, O_WRONLY);
        sprintf(filebuf,"%d:[%s]\n", getpid (), buf);
        write(fd,filebuf,sizeof(filebuf));
        close(fd);

        printf ("type to produce log: ");
    }
 
    unlink(myfifo);
    exit (0);
}

