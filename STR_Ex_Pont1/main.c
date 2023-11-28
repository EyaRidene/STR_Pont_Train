#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void* voiture(void* arg);
void* camion(void* arg);

sem_t attendre_voiture;
sem_t attendre_camion;


int nb_camion=0;
int nb_voiture=0;
int poid_sur_pont=0;

pthread_mutex_t mutex;

void attendre(double max);

void acceder_au_pont(int poid)
{
    pthread_mutex_lock(&mutex);

    if(poid_sur_pont+poid <= 15)
    {


        poid_sur_pont+=poid;
        if (poid==5)
            sem_post(&attendre_voiture);
        else	sem_post(&attendre_camion);
    }
    else	{

        if (poid==5)
            nb_voiture++;
        else	nb_camion++;
    }

    pthread_mutex_unlock(&mutex);

    if(poid==5)
        sem_wait(&attendre_voiture);
    else	sem_wait(&attendre_camion);
}

void quitter_le_pont(int poid)
{
    pthread_mutex_lock(&mutex);


    poid_sur_pont-=poid;


    if ( (poid_sur_pont==0) && (nb_camion!=0) )
    {
        nb_camion--;
        sem_post(&attendre_camion);
        poid_sur_pont=15;
    }
    else

        while ( (poid_sur_pont < 15) && (nb_voiture>0) && (nb_camion==0) )
        {
            nb_voiture--;
            poid_sur_pont+=5;
            sem_post(&attendre_voiture);
        }

    pthread_mutex_unlock(&mutex);
}

void* voiture(void* arg)
{
    int pid=*((int*)arg);

    usleep(1000000);
    acceder_au_pont(5);
    printf("Le Voiture %d occupe le pont\n",pid);
    usleep(1000000);
    printf("Le voiture %d libere le pont\n",pid);
    quitter_le_pont(5);

    pthread_exit(NULL);
}

void* camion(void* arg)
{
    int pid=*((int*)arg);

    usleep(1000000);
    acceder_au_pont(15);
    printf("le ccamion %d occupe le pont\n",pid);
    usleep(1000000);
    printf("Le Camion %d libere le pont\n",pid);
    quitter_le_pont(15);

    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    srand(time(NULL));
    pthread_t id;

    sem_init(&attendre_voiture,0,0);
    sem_init(&attendre_camion,0,0);
    pthread_mutex_init(&mutex,0);

    for (int i=0;i<10;i++){
        int vehicule = rand() % 2;
        int* j = (int*)malloc(sizeof(int));
        *j=i;
        if (vehicule==1){
            pthread_create(&id,NULL,camion,j);
        }
        else{
            pthread_create(&id,NULL,voiture,j);
        }}


    pthread_exit(NULL);
};

