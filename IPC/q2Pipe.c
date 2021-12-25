
#define PIPE_NUMBER 1
#define NUMBER 10
#define SLEEPSIZE 10
#define STR1SIZE 100
#define STR2SIZE 90
#define FILENAMEOUPUT "./result.txt"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

FILE *file_pointer;
bool check_delete_file = false;
void randome_string_generator(char *dest, size_t length);
int create_log_function(int id, int write_pipe);
int logger(int read_pipe);

int main()
{
    // if (remove(FILENAMEOUPUT) == 0 && !check_delete_file)
    // {
    //     printf("The file is deleted successfully.");
    //     check_delete_file = true;
    // }

    // else
    //     printf("The file is not deleted.");

    int pipes[PIPE_NUMBER][2];
    for (size_t i = 0; i < PIPE_NUMBER; i++)
    {
        pipe(pipes[i]);
    }

    int pid1;
    pid1 = fork();
    if (pid1 == 0)
    {
        pid1 = fork();
        if (pid1 != 0)
        {
            create_log_function(pid1, pipes[0][1]);
        }
        else
        {
            pid1 = fork();
            if (pid1 != 0)
            {
                create_log_function(pid1, pipes[0][1]);
            }
            else
            {
                pid1 = fork();
                if (pid1 != 0)
                    create_log_function(pid1, pipes[0][1]);
            }
        }
        exit(0);
    }
    else
        logger(pipes[0][0]);

    exit(0);
}

void randome_string_generator(char *dest, size_t length)
{
    char array_char[] = "0123456789"
                        "!@#$&*()_+/*"
                        "abcdefghijklmnopqrstuvwxyz";
    unsigned long ARRAYSIZE = sizeof array_char - 1;
    int last_index = 0;
    for (int i = 0; i < length; i++)
    {
        size_t index = (double)rand() / RAND_MAX * (ARRAYSIZE);
        dest[i] = array_char[index];
        last_index = i;
    }
    dest[last_index] = '\0';
}

int create_log_function(int id, int write_pipe)
{
    char str1[STR1SIZE];
    char str2[STR2SIZE];
    sleep(SLEEPSIZE);
    for (int i = 0; i < NUMBER; i++)
    {
        randome_string_generator(str2, 10 + i + id % 10);
        sprintf(str1, "%d : %s", id, str2);
        write(write_pipe, &str1, sizeof(char) * STR2SIZE);
        sleep(SLEEPSIZE);
    }
    return 1;
}

int logger(int read_pipe)
{
    char str2[80];
    int count = 0;
    file_pointer = fopen(FILENAMEOUPUT, "w");
    printf("start : logger 0\n");
    for (size_t i = 0; i < NUMBER * 3; i++)
    {
        read(read_pipe, &str2, sizeof(char) * 80);
        count++;
        printf("%d => %s\n", count, str2);
        fprintf(file_pointer, "%s\n", str2);
    }
    fclose(file_pointer);
}