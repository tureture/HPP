#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * do_stuff(void* arg){
    int x = 1;
    for(int i = 0; i < 1000000; i++){
        x = x * (x+1) / (x);
    }
    printf("Done inner \n");
    return NULL;
}

void * spawn2(void* arg) {

    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, do_stuff, NULL);
    pthread_create(&thread2, NULL, do_stuff, NULL);

    printf("Child threads created \n");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Done outer \n");
    return NULL;
}

int main(){

    pthread_t thread1;
    pthread_t thread2;


    pthread_create(&thread1, NULL, spawn2, NULL);
    pthread_create(&thread2, NULL, spawn2, NULL);

    printf("Main threads created \n");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Done main \n");

    return 0;
}