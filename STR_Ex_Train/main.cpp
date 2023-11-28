#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void* MR1(void* arg); //A->D
void* MR2(void* arg); //F->A
void* MR3(void* arg); //E->B

//mutex pour chaque segment
pthread_mutex_t mut_AB;
pthread_mutex_t mut_BC;
pthread_mutex_t mut_CD;
pthread_mutex_t mut_EB;
pthread_mutex_t mut_FA;

//Demander l'acc�s � un segment
void entrer_segment(char depart, char arrivee)
{
    if( depart == 'A' && arrivee =='B' ){
        pthread_mutex_lock(&mut_AB);
    }
    else if( depart == 'B' && arrivee =='C' ){
        pthread_mutex_lock(&mut_BC);
    }
    else if( depart == 'C' && arrivee =='D' ){
        pthread_mutex_lock(&mut_CD);
    }
    else if( depart == 'E' && arrivee =='B' ){
        pthread_mutex_lock(&mut_EB);
    }
    else{
        pthread_mutex_lock(&mut_FA);
    }

}

//Liberer un segment
void sortir_segment(char depart, char arrivee)
{
    if( depart == 'A' && arrivee =='B' ){
        pthread_mutex_unlock(&mut_AB);
    }
    else if( depart == 'B' && arrivee =='C' ){
        pthread_mutex_unlock(&mut_BC);
    }
    else if( depart == 'C' && arrivee =='D' ){
        pthread_mutex_unlock(&mut_CD);
    }
    else if( depart == 'E' && arrivee =='B' ){
        pthread_mutex_unlock(&mut_EB);
    }
    else{
        pthread_mutex_unlock(&mut_FA);
    }
}


//A->D
void* MR1(void* arg)
{
    entrer_segment('A', 'B');
    printf("Train %d est dans le segment AB \n", *((int*)(&arg)));
    usleep(600000);
    sortir_segment('A', 'B');

    entrer_segment('B', 'C');
    printf("Train %d est dans le segment BC \n", *((int*)(&arg)));
    usleep(600000);
    sortir_segment('B', 'C');

    entrer_segment('C', 'D');
    printf("Train %d est dans le segment CD \n", *((int*)(&arg)));
    usleep(600000);
    sortir_segment('C', 'D');

}

//F->A
void* MR2(void* arg)
{
    entrer_segment('F', 'A');
    printf("Train %d est dans le segment FA \n",*((int*)(&arg)));
    usleep(600000);
    sortir_segment('F', 'A');

}

//E->B
void* MR3(void* arg)
{
    entrer_segment('E', 'B');
    printf("Train %d est dans le segment EB \n",*((int*)(&arg)));
    usleep(600000);
    sortir_segment('E', 'B');

}

int main(int argc, char* argv[])
{
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    pthread_t t5;

    pthread_mutex_init(&mut_AB,NULL);
    pthread_mutex_init(&mut_BC,NULL);
    pthread_mutex_init(&mut_CD,NULL);
    pthread_mutex_init(&mut_EB,NULL);
    pthread_mutex_init(&mut_FA,NULL);


    pthread_create(&t1, NULL, MR1,(void*) 1);
    pthread_create(&t2, NULL, MR2,(void*) 2);
    pthread_create(&t3, NULL, MR3,(void*) 3);
    pthread_create(&t4, NULL, MR1,(void*) 4);
    pthread_create(&t5, NULL, MR2,(void*) 5);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);
    pthread_join(t5,NULL);
}
