#include "logheader.h"

void log_preprocess(char buf[], int length){
    if (buf [length - 1] == '\n')
           buf [length - 1] = '\0';
}


//TODO: close open file
void error (char *msg)
{
    perror (msg);
    exit (1);
}

int * get_number(long* pos)
{

    FILE* file = fopen(MKPIPE, "r");
    char line[256];

    if (file==NULL) 
    {
        perror ("Error reading file");
    }
    else
    {
        fseek(file , *pos , SEEK_CUR);
        fgets(line, sizeof(line), file);
        printf("%s", line);
    }
    *pos = ftell(file);
    return 0;
}

