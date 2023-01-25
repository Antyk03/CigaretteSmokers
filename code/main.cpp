#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>

using namespace std;

sem_t mutex;

void* threadAction(void* arg) {
    int num = *(int*) arg;
    
    //aspetta
    sem_wait(&mutex);
    cout<<"\nEntered... "<< num << "° thread\n";
    
    sleep(1);
    
    //segnala
    cout<<"\nJust Exiting...\n";
    sem_post(&mutex);
}

int main() {
    int num = 1;
    int num2 = 2;
    sem_init(&mutex, 0, 1);
    //creazione Threads
    pthread_t t1, t2;
    pthread_create(&t1, NULL, threadAction, (void*) &num);
    pthread_create(&t2, NULL, threadAction, (void*) &num2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    sem_destroy(&mutex);
    return 0;
}