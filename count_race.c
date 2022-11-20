#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
struct argument{int start, end,id;};
struct argument *Arg;

int *array;
long length;
int count;
int NTHREADS;
void * count1s();

int main() {
    struct timeval start, end;
    int counter1s = 0;
    srand(time(NULL));
    printf ("Size of array: ");
    scanf("%d", &length);
    printf("Number of Threads: ");
    scanf("%d", &NTHREADS);
    pthread_t pthreads[NTHREADS];
    array = (int*) malloc(length*sizeof(int));// allocate space to array based on user input for length.

    for (int i = 0; i<length; i++){
        array[i] = (rand()%4); // Fill array with elements between 0 and 3
        if (*(array + i) == 1){ // Find the occurences of 1 in the array.
            counter1s++;
        }
    }
    gettimeofday(&start, 0);
    for (int i = 0; i < NTHREADS; i++) {
        Arg = malloc(3 * sizeof (int));
        Arg->start = i * floor((double)length/NTHREADS);
        Arg->end = Arg->start + floor((double)length / NTHREADS);
        Arg->id = i;
        pthread_create(&pthreads[i], NULL, count1s, (void *) Arg);
    }
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(pthreads[i], NULL);
    }
    gettimeofday(&end, 0);

    long seconds = end.tv_sec - start.tv_sec;
    long miliseconds = end.tv_usec - start.tv_usec;
    double totalElapsedTime = seconds + miliseconds * pow(10, -6);
    free(array);
    //Print the final results to be recorded. 

    printf("\n");
    printf("Final Results: \n");
    printf("Actual # of Ones: %d\n", counter1s);
    printf("Race Condition Result: %d\n", count);
    printf("Time Elapsed: %f\n", totalElapsedTime);
}

/*
Requires: void pointer 
Effect: Returns the number of 1s found with race condition
*/
void* count1s (void * start) {
    int i;
    struct argument* args = (struct argument*) start;
    if(args->id == NTHREADS - 1) {
        for (i = args->start; i < length; i++) {
            if (array[i] == 1) {
                count++;
            }
        }
    }
    else {
        for (i = args->start; i < args->end; i++) {
            if (array[i] == 1) {
                count++;
            }
        }
    }
    free(start);
}