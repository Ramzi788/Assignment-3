#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include<stdlib.h>

int *array;
long length;
int count;
int NTHREADS;
const int fillCache = 64000-16000;
void * count1s();
struct argument{int count, start, end, val; char dummy[fillCache];};

int main() {
    struct timeval start, end;
    struct argument* arg;
    int counter1s = 0;

    //User Input
    srand(time(NULL));
    printf("Please enter the size of the array: ");
    scanf("%ld", &length);
    printf("Enter Number of Threads: ");
    scanf("%d", &NTHREADS);
    pthread_t pthreads[NTHREADS];

    array = (int *) malloc(length * sizeof(int));
    for (int i = 0; i < length; i++)
        array[i] = (rand() % 6);
    for (int i = 0; i < length; i++) {
        if (array[i] == 1)
            counter1s++;
    }
    
    
    gettimeofday(&start, 0);
    for (int i = 0; i < NTHREADS; i++) {
        arg = malloc(4 * sizeof(int) + fillCache);
        arg->count = 0;
        arg->start = i * length/NTHREADS;
        arg->end = arg->start + length/NTHREADS;
        arg->val = i;
        for(int i = 0; i < fillCache; i++){
            arg->dummy[i] = 'a';
        }
        pthread_create(&pthreads[i], NULL, count1s, (void *) arg);
    }
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(pthreads[i], (void**) &arg);
        count += arg->count;
        free(arg);
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - start.tv_sec;
    long miliseconds = end.tv_usec - start.tv_usec;
    double totalElapsedTime = seconds + miliseconds * pow(10, -6);
    free(array);

    //Print Results
    printf("\n");
    printf("Final Results: \n");
    printf("Actual # of Ones: %d\n", counter1s);
    printf("Race Condition Result: %d\n", count);
    printf("Time Elapsed: %f\n", totalElapsedTime);
}
/*
Requires: void pointer
Effect: Return number of 1s occured. 
*/
void* count1s (void*arg) {
    struct argument* Arg = (struct argument*)arg;
    int i;
    if(Arg->val == NTHREADS - 1){
        for (i = Arg->start; i < length; i++) {
            if (array[i] == 1) {
                Arg->count++;
            }
        }
    }
    else {
        for (i = Arg->start; i < Arg->end; i++) {
            if (array[i] == 1) {
                Arg->count++;
            }
        }
    }
    return (void*) Arg;
}