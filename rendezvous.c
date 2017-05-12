/**
 * Rendezvous
 *
 * Two threads executing chunks of work in a lock step - skeleton
 * code.
 *
 * Author: Nikos Nikoleris <nikos.nikoleris@it.uu.se>
 *
 */

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* abort(), [s]rand() */
#include <unistd.h>    /* sleep() */
#include <semaphore.h> /* sem_...() */
#include <pthread.h>   /* pthread_...() */

#define LOOPS 5
#define NTHREADS 3
#define MAX_SLEEP_TIME 3 


/* TODO: Make the two threads perform their iterations in a
 * predictable way. Both should perform iteration 1 before iteration 2
 * and then 2 before 3 etc. */

sem_t semA, semB; //semaphore for thread A and B


/*
 Alternatively lock & unlock each thread to make it wokr in order (ex. A0->B0->B1->A1->B2->A2->...)
 */

void *
threadA(void *param __attribute__((unused)))
{
    int i, val;

    for (i = 0; i < LOOPS; i++) {

        sem_wait(&semA); //decrease semA (if it is not positive number, wait until it is increased)

        printf("threadA --> %d iteration\n", i);

        sem_post(&semB); //increase semB so that it can perform the next operation

        sleep(rand() % MAX_SLEEP_TIME);
  }

    pthread_exit(0);
}


void *
threadB(void *param  __attribute__((unused)))
{
    int i, val;

    for (i = 0; i < LOOPS; i++) {
        sem_wait(&semB);    //decrease semB (if it is not positive number, wait until it is increased)

        printf("threadB --> %d iteration\n", i);
        
        sem_post(&semA);    //increase semA so that it can perform the next operation
    
        sleep(rand() % MAX_SLEEP_TIME);
    }

    pthread_exit(0);
}

int
main()
{
    pthread_t tidA, tidB;

    srand(time(NULL));
    pthread_setconcurrency(3);

    /*Initialize semaphores
     If either of them fails, print error msg and abort the program*/
    if(sem_init(&semA, 0, 1) || sem_init(&semB, 0, 1)){
        perror("semaphore initialization error\n");
        abort();
    }

    if (pthread_create(&tidA, NULL, threadA, NULL) ||
	pthread_create(&tidB, NULL, threadB, NULL)) {
	perror("pthread_create");
	abort();
    }
    if (pthread_join(tidA, NULL) != 0 ||
        pthread_join(tidB, NULL) != 0) {
	perror("pthread_join");
	abort();
    }


    return 0;
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * c-file-style: "stroustrup"
 * End:
 */
