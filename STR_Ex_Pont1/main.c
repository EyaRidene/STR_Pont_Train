#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Function prototypes
void* car(void* arg);
void* truck(void* arg);

// Semaphores and counters
sem_t carSemaphore;
sem_t truckSemaphore;

int truckCount = 0;
int carCount = 0;
int weightOnBridge = 0;

// Mutex for shared resource access
pthread_mutex_t bridgeMutex;

// Function to control access to the bridge based on vehicle weight
void enterBridge(int weight)
{
    pthread_mutex_lock(&bridgeMutex);

    if (weightOnBridge + weight <= 15)
    {
        weightOnBridge += weight;
        if (weight == 5)
            sem_post(&carSemaphore);
        else
            sem_post(&truckSemaphore);
    }
    else
    {
        if (weight == 5)
            carCount++;
        else
            truckCount++;
    }

    pthread_mutex_unlock(&bridgeMutex);

    if (weight == 5)
        sem_wait(&carSemaphore);
    else
        sem_wait(&truckSemaphore);
}

// Function to simulate leaving the bridge and adjusting counters
void leaveBridge(int weight)
{
    pthread_mutex_lock(&bridgeMutex);

    weightOnBridge -= weight;

    if ((weightOnBridge == 0) && (truckCount != 0))
    {
        truckCount--;
        sem_post(&truckSemaphore);
        weightOnBridge = 15;
    }
    else
    {
        while ((weightOnBridge < 15) && (carCount > 0) && (truckCount == 0))
        {
            carCount--;
            weightOnBridge += 5;
            sem_post(&carSemaphore);
        }
    }

    pthread_mutex_unlock(&bridgeMutex);
}

// Function representing the behavior of a car
void* car(void* arg)
{
    int pid = *((int*)arg);

    usleep(1000000);
    enterBridge(5);
    printf("Car %d occupies the bridge\n", pid);
    usleep(1000000);
    printf("Car %d releases the bridge\n", pid);
    leaveBridge(5);

    pthread_exit(NULL);
}

void* truck(void* arg)
{
    int pid = *((int*)arg);

    usleep(1000000);
    enterBridge(15);
    printf("Truck %d occupies the bridge\n", pid);
    usleep(1000000);
    printf("Truck %d releases the bridge\n", pid);
    leaveBridge(15);

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    pthread_t threadId;

    // Initialize semaphores and mutex
    sem_init(&carSemaphore, 0, 0);
    sem_init(&truckSemaphore, 0, 0);
    pthread_mutex_init(&bridgeMutex, 0);

    // Create threads for vehicles
    for (int i = 0; i < 10; i++)
    {
        int vehicleType = rand() % 2;
        int* vehicleId = (int*)malloc(sizeof(int));
        *vehicleId = i;

        if (vehicleType == 1)
        {
            pthread_create(&threadId, NULL, truck, vehicleId);
        }
        else
        {
            pthread_create(&threadId, NULL, car, vehicleId);
        }
    }

    pthread_exit(NULL);
}
