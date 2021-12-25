#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define PN 16

void mergeSort(int arr[], int l, int h);
void merge(int a[], int l1, int h1, int h2);
int *p_count; 
void mergeSort_parallel(int a[], int l, int h)
{
    int i, len=(h-l+1);
    //TODO stop based on number of created process
    if (p_count[0]>=PN)
    {
	 mergeSort(a, l,h);
        return;
    }
 
    pid_t lpid,rpid;
    p_count[0]+=2; // count optimistically
    lpid = fork();
    if (lpid==0)
    {
        mergeSort_parallel(a,l,l+len/2-1);
        _exit(0);
    }
    else
    {

        if(rpid==0)
        {
            mergeSort_parallel(a,l+len/2,h);
            _exit(0);
        }
    }
 
    int status;
 
    // Wait for child processes to finish
    waitpid(lpid, &status, 0);
    waitpid(rpid, &status, 0);
 
    // Merge the sorted subarrays
    merge(a, l, l+len/2-1, h);
}
 
/* merge sort sequential*/
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
  
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
  
        merge(arr, l, m, r);
    }
}
 
// Method to merge sorted subarrays
void merge(int a[], int l1, int h1, int h2)
{

    int count=h2-l1+1;
    int sorted[count];
    int i=l1, k=h1+1, m=0;
    while (i<=h1 && k<=h2)
    {
        if (a[i]<a[k])
            sorted[m++]=a[i++];
        else if (a[k]<a[i])
            sorted[m++]=a[k++];
        else if (a[i]==a[k])
        {
            sorted[m++]=a[i++];
            sorted[m++]=a[k++];
        }
    }
 
    while (i<=h1)
        sorted[m++]=a[i++];
 
    while (k<=h2)
        sorted[m++]=a[k++];
 
    int arr_count = l1;
    for (i=0; i<count; i++,l1++)
        a[l1] = sorted[i];
}
 

 
// To fill randome values in array for testing
// purpose
void fillData(int a[], int len)
{
    // Create random arrays
    int i;
    for (i=0; i<len; i++)
        a[i] = rand();
    return;
}
 
void printArr(int arr[] , int size){
    printf("printing array:\n");
    int i=0;
    for(i=0;i<size;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
} 
// Driver code
int main()
{
    int shmid;
    key_t key = IPC_PRIVATE;
    int *shm_array;
 
    p_count = (int *) malloc(sizeof(int)*1);
    p_count[0]=0;

    int length = 16;
 

    size_t SHM_SIZE = sizeof(int)*length;
 
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        _exit(1);
    }
 
    if ((shm_array = shmat(shmid, NULL, 0)) == (int *) -1)
    {
        perror("shmat");
        _exit(1);
    }
 
    srand(time(NULL));
    fillData(shm_array, length);
 
    mergeSort_parallel(shm_array, 0, length-1);
 
    // Check if array is sorted or not
    printArr(shm_array, length);
 
    /* Detach from the shared memory now that we are
       done using it. */
    if (shmdt(shm_array) == -1)
    {
        perror("shmdt");
        _exit(1);
    }
 
    /* Delete the shared memory segment. */
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        _exit(1);
    }
 
    return 0;
}
