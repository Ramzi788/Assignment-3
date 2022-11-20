#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

int *array;
int length;
int count;
void * count1();
int NTHREADS;
int totalCount;

struct argument{int start, end, val;};


int main(){ 
    struct timeval beginTimer, finishTimer; 
    int counter1s = 0;
    srand(time(NULL));
    
    printf ("Size of array: ");
    scanf("%d", &length);
    printf("Number of Threads: ");
    scanf("%d", &NTHREADS);
    pthread_t pthreads[NTHREADS];
    int * result;//Result of Count1
    array = (int*) malloc(length*sizeof(int));// allocate space to array based on user input for length.

    for (int i = 0; i<length; i++){
        array[i] = (rand()%4); // Fill array with elements between 0 and 3
        if (*(array + i) == 1){ // Find the occurences of 1 in the array.
            counter1s++;
        }
    }
    gettimeofday(&beginTimer,0);


    for (int i = 0; i < NTHREADS; i++) {
        struct argument* args = malloc(3 * sizeof (int));
        args->start = i * length/NTHREADS;
        args->end = args->start + length/NTHREADS;
        args->val = i;
        pthread_create(&pthreads[i], NULL, count1, (void *) args);
    }
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(pthreads[i], (void**) &result);
        totalCount += *result;
        free(result);
    }
    gettimeofday(&finishTimer, 0); // Stop timing

    long seconds = finishTimer.tv_sec - beginTimer.tv_sec;
    long miliseconds = finishTimer.tv_usec - beginTimer.tv_usec;
    double totalElapsedTime = seconds + miliseconds*pow(10, -6); //Convert ms to s
    free(array); // Free allocated space from the array

    //Print the final results to be recorded. 

    printf("\n");
    printf("Final Results: \n");
    printf("Actual # of Ones: %d\n", counter1s);
    printf("Race Condition Result: %d\n", totalCount);
    printf("Time Elapsed: %f\n", totalElapsedTime);

}
/*
Requires: void pointer 
Effect: Returns the number of 1s found, using a private variable counter that can't be accessed. 
*/
void * count1 (void * arg){ // Race Condition occurs due to multi threading.
    struct argument * arg1 = (struct argument*) arg;
    int counter = 0;
    int * ptr = malloc(sizeof(int));
    if (arg1->val == NTHREADS-1){//Reached the end of all the threads.
        arg1->end = length; 
    }
    for (int i = arg1->start; i < arg1->end; i++){
        if (array[i]==1){
            counter ++;
        }    
    }
    free(arg);// Free up space for address from stack
    *ptr = counter;
    return (void*) ptr;
}
