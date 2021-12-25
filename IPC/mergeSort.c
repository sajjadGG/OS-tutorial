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

 
/* merge sort sequential*/
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {

        int m = l + (r - l) / 2;
  
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
  
        merge(arr, l, m, r);
    }
}

void mergeSort_parallel(int a[], int l, int h)
{
    int i, len=(h-l+1);
    
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
 
    waitpid(lpid, &status, 0);
    waitpid(rpid, &status, 0);

    merge(a, l, l+len/2-1, h);
}
 
void merge(int a[], int l1, int h1, int h2)
{
    int cand_i , cand_j;
    int len=h2-l1+1;
    int res[len];
    int i=l1, k=h1+1, m=l1;
    while (i<=h1 && k<=h2)
    {   
        cand_i = a[i];
        cand_j = a[k];
        if (cand_i<=cand_j){
            a[m++]=cand_i;
            i++;
        }
        else{
            a[m++]=cand_j;
            k++;
        }
 
    }
 
    //left overs
    while (i<=h1)
        a[m++]=a[i++];
 
    while (k<=h2)
        a[m++]=a[k++];
 
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
 
    shm_array = {12,25,36,78,2,3,5,6,8,11,0,135,55,44,32,69};
 
    mergeSort_parallel(shm_array, 0, length-1);
 

    printArr(shm_array, length);
 

    if (shmdt(shm_array) == -1)
    {
        perror("shmdt");
        _exit(1);
    }
 
 
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        _exit(1);
    }
 
    return 0;
}
