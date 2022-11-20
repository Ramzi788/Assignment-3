#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
struct argument{int start, end, val;};
pthread_mutex_t l;

struct argument *Arg;
int *array;
long length;
int count;
int NTHREADS;
void * count1s();


int main() {
    struct timeval beginTime, finishTime;
    int counter1s = 0;
    //User Input
    srand(time(NULL));
    printf("Please enter the size of the array: ");
    scanf("%ld", &length);
    printf("Enter Number of Threads: ");
    scanf("%d", &NTHREADS);
    pthread_t pthreads[NTHREADS];
    pthread_mutex_init(&l, NULL);

    array = (int *) malloc(length * sizeof(int));

    for (int i = 0; i < length; i++){
        array[i] = (rand() % 4);
        if (array[i] == 1){
            counter1s++;
        }
    }
    gettimeofday(&beginTime, 0); //Begin timer
    for (int i = 0; i < NTHREADS; i++) {
        Arg = malloc(3 * sizeof (int));
        Arg->start = i * floor((double)length/NTHREADS);
        Arg->end = Arg->start + floor((double)length / NTHREADS);
        Arg->val = i;
        pthread_create(&pthreads[i], NULL, count1s, (void *) Arg);
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(pthreads[i], NULL);
    }
    pthread_mutex_destroy(&l);//Were done using the lock, so delete it.
    gettimeofday(&finishTime, 0); // Stop Timer 

    //Calculate Time Elapsed
    long seconds = finishTime.tv_sec - beginTime.tv_sec;
    long miliseconds = finishTime.tv_usec - beginTime.tv_usec;
    double totalElapsedTime = seconds + miliseconds * pow(10, -6);//Conversion from ms to s

    free(array);//Free array data from stack memory

    printf("\n");
    printf("Final Results: \n");
    printf("Actual # of Ones: %d\n", counter1s);
    printf("Race Condition Result: %d\n", count);
    printf("Time Elapsed: %f\n", totalElapsedTime);
}
/*
Requires: void pointer
Effect: Uses Mutex Locking to avoid race conditions and returns the occurences of 1 in the array
*/
void* count1s (void* arg) {
    struct argument* args = (struct argument*) arg;
    if(args->val == NTHREADS - 1) {
        for (int i = args->start; i < length; i++) {
            if (array[i] == 1) {
                pthread_mutex_lock(&l);
                count++;
                pthread_mutex_unlock(&l);
            }
        }
    }
    else {
        for (int i = args->start; i < args->end; i++) {
            if (array[i] == 1) {
                pthread_mutex_lock(&l);
                count++;
                pthread_mutex_unlock(&l);
            }
        }
    }
    free(arg);
}