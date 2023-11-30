#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* MR1(void* arg);
void* MR2(void* arg);
void* MR3(void* arg);

pthread_mutex_t mut_AB;
pthread_mutex_t mut_BC;
pthread_mutex_t mut_CD;
pthread_mutex_t mut_FB;
pthread_mutex_t mut_BA;
pthread_mutex_t mut_EC;
pthread_mutex_t mut_CB;

void enterSegment(char depart, char arrivee)
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
    else if( depart == 'E' && arrivee =='C' ){
        pthread_mutex_lock(&mut_EC);
    }
    else if( depart == 'C' && arrivee =='B' ){
        pthread_mutex_lock(&mut_CB);
    }
    else if( depart == 'F' && arrivee =='B' ){
        pthread_mutex_lock(&mut_FB);
    }
    else{
        pthread_mutex_lock(&mut_BA);
    }

}

void outSegment(char depart, char arrivee)
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
    else if( depart == 'E' && arrivee =='C' ){
        pthread_mutex_unlock(&mut_EC);
    }
    else if( depart == 'C' && arrivee =='B' ){
        pthread_mutex_unlock((&mut_CB));
    }
    else if( depart == 'F' && arrivee =='B' ){
        pthread_mutex_unlock(&mut_FB);
    }
    else{
        pthread_mutex_unlock(&mut_BA);
    }
}

void* MR1(void* arg)
{
    enterSegment('A', 'B');
    printf("Metro %d est dans le segment AB \n", *((int*)(&arg)));
    usleep(600000);
    outSegment('A', 'B');

    enterSegment('B', 'C');
    printf("Metro %d est dans le segment BC \n", *((int*)(&arg)));
    usleep(600000);
    outSegment('B', 'C');

    enterSegment('C', 'D');
    printf("Metro %d est dans le segment CD \n", *((int*)(&arg)));
    usleep(600000);
    outSegment('C', 'D');

}

void* MR2(void* arg)
{
    enterSegment('F', 'B');
    printf("Metro %d est dans le segment FB \n",*((int*)(&arg)));
    usleep(600000);
    outSegment('F', 'B');
    enterSegment('B', 'A');
    printf("Metro %d est dans le segment BA \n",*((int*)(&arg)));
    usleep(600000);
    outSegment('B', 'A');

}

void* MR3(void* arg)
{
    enterSegment('E', 'C');
    printf("Metro %d est dans le segment EC \n",*((int*)(&arg)));
    usleep(600000);
    outSegment('E', 'C');
    enterSegment('C', 'B');
    printf("Metro %d est dans le segment CB \n",*((int*)(&arg)));
    usleep(600000);
    outSegment('C', 'B');

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
    pthread_mutex_init(&mut_EC,NULL);
    pthread_mutex_init(&mut_CB,NULL);
    pthread_mutex_init(&mut_BA,NULL);
    pthread_mutex_init(&mut_FB,NULL);
    pthread_mutex_init(&mut_BA,NULL);


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
