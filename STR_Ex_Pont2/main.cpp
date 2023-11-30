#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define TRUCK 1
#define CAR 0

sem_t bridgeSem;
pthread_mutex_t truckMutex;

void enterBridge(int vehicle)
{
    if (vehicle == TRUCK)
    {
        pthread_mutex_lock(&truckMutex);

        sem_wait(&bridgeSem);
        sem_wait(&bridgeSem);
        sem_wait(&bridgeSem);

        pthread_mutex_unlock(&truckMutex);
    }
    else
    {
        sem_wait(&bridgeSem);
    }
}

void leaveBridge(int vehicle)
{
    if (vehicle == TRUCK)
    {
        sem_post(&bridgeSem);
        sem_post(&bridgeSem);
        sem_post(&bridgeSem);
    }
    else
    {
        sem_post(&bridgeSem);
    }
}

void* car(void *arg)
{
    int pid = *((int *)arg);

    usleep(1000000);
    enterBridge(CAR);
    printf("Car %d occupies the bridge\n", pid);
    usleep(1000000);
    printf("Car %d releases the bridge\n", pid);
    leaveBridge(CAR);

    pthread_exit(NULL);
}

void *truck(void *arg)
{
    int pid = *((int *)arg);

    usleep(1000000);
    enterBridge(TRUCK);
    printf("Truck %d occupies the bridge\n", pid);
    usleep(1000000);
    printf("Truck %d releases the bridge\n", pid);
    leaveBridge(TRUCK);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t id;

    sem_init(&bridgeSem, 0, 3);

    for (int i = 0; i < 10; i++)
    {
        int vehicle = rand() % 2;
        int *j = (int *)malloc(sizeof(int));
        *j = i;

        if (vehicle == TRUCK)
        {
            pthread_create(&id, NULL, truck, j);
        }
        else
        {
            pthread_create(&id, NULL, car, j);
        }
    }

    pthread_exit(NULL);
}
