#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>

using namespace std;

// Creating semaphores to coordinate the smokers.
sem_t match_sem, paper_sem, tobacco_sem, more_needed;

//Initialize Semaphores
void initSemaphores() {
    sem_init(&match_sem, 0, 0); // Initialize the match semaphore to 0
    sem_init(&paper_sem, 0, 0);
    sem_init(&tobacco_sem, 0, 0);
    sem_init(&more_needed, 0, 0);
}

// Smokers threads
void* smoker_tobacco(void *arg) {
    while (true) {
        sem_wait (&match_sem);
        cout << "\nSmoker with tobacco grab match from table.";
        if (sem_trywait (&paper_sem) == 0) {
            cout << "\nSmoker with tobacco grab paper from table.";
            cout << "\nSmoker with tobacco is smoking...";
            cout << "\nSmoker with tobacco request for more.";
            sem_post(&more_needed);
        }
        else {
            sem_post(&match_sem);
            cout << "\nSmoker with tobacco drop match on table.";
        }
        sleep(2);
    }
}

void* smoker_match(void *arg) {
    while (true) {
        sem_wait (&paper_sem);
        cout << "\nSmoker with match grab paper from table.";
        if (sem_trywait (&tobacco_sem) == 0) {
            cout << "\nSmoker with match grab tobacco from table.";
            cout << "\nSmoker with match is smoking...";
            cout << "\nSmoker with match request for more.";
            sem_post(&more_needed);
        }
        else {
            sem_post(&paper_sem);
            cout << "\nSmoker with match drop paper on table.";
        }
        sleep(2);
    }
}

void* smoker_paper(void *arg) {
    while (true) {
        sem_wait (&tobacco_sem);
        cout << "\nSmoker with paper grab tobacco from table.";
        if (sem_trywait (&match_sem) == 0) {
            cout << "\nSmoker with paper grab match from table.";
            cout << "\nSmoker with paper is smoking...";
            cout << "\nSmoker with paper request for more.";
            sem_post(&more_needed);
        }
        else {
            sem_post(&tobacco_sem);
            cout << "\nSmoker with paper drop tobacco on table.";
        }
        sleep(2);
    }
}
//

// Agent thread
void* agent (void *arg) {
    while (true) {
        int number = rand() % 3;
        if (number == 0) {
            sem_post(&match_sem);
            cout << "\nAgent drop match on table.";
            sem_post(&paper_sem);
            cout << "\nAgent drop paper on table.";
        } else if (number == 1) {
            sem_post(&match_sem);
            cout << "\nAgent drop match on table.";
            sem_post(&tobacco_sem);
            cout << "\nAgent drop tobacco on table.";
        } else if (number == 2) {
            sem_post(&tobacco_sem);
            cout << "\nAgent drop tobacco on table.";
            sem_post(&paper_sem);
            cout << "\nAgent drop paper on table.";
        }
        sem_wait (&more_needed);   
    }
}
//

//Creating the threads
pthread_t agentThread, smokerTobaccoThread, smokerMatchThread, smokerPaperThread;

int createThreads() {
    int error = 0;
    error = error + pthread_create(&smokerMatchThread, NULL, smoker_tobacco, NULL);
    error = error + pthread_create(&smokerTobaccoThread, NULL, smoker_paper, NULL);
    error = error + pthread_create(&smokerPaperThread, NULL, smoker_match, NULL);
    error = error + pthread_create(&agentThread, NULL, agent, NULL);
    return error;
}


//Joining threads with main thread
void joinThreads() {
    pthread_join(agentThread, NULL);
    pthread_join(smokerTobaccoThread, NULL);
    pthread_join(smokerMatchThread, NULL);
    pthread_join(smokerPaperThread, NULL);
}

//Destroying semaphores
void destroySemaphores() {
    sem_destroy(&match_sem);
    sem_destroy(&paper_sem);
    sem_destroy(&tobacco_sem);
    sem_destroy(&more_needed);
}

int main() {
    cout <<"\n------------Cigarette Smokers Problem in C++------------------\n\n";
    initSemaphores(); 
    if (createThreads() > 0) {
        cout << "Error creating threads.\n";
        return 0;
    }
    joinThreads();
    destroySemaphores();
    return 0;
}