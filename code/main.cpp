#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>

using namespace std;

// Creating semaphores to coordinate the smokers.
sem_t match_sem, paper_sem, tobacco_sem, more_needed;

// Smokers threads
void* smoker_tobacco(void *arg) {
    while (true) {
        sem_wait (&match_sem);
        cout << "Smoker with tobacco grab match from table." << endl;
        if (sem_trywait (&paper_sem) == 0) {
            cout << "Smoker with tobacco grab paper from table." << endl;
            sem_post(&more_needed);
            cout << "Smoker with tobacco request for more." << endl;
        }
        else {
            sem_post(&match_sem);
            cout << "Smoker with tobacco drop match on table." << endl;
        }
        sleep(1);
    }
}

void* smoker_match(void *arg) {
    while (true) {
        sem_wait (&tobacco_sem);
        cout << "Smoker with match grab tobacco from table." << endl;
        if (sem_trywait (&paper_sem) == 0) {
            cout << "Smoker with match grab paper from table." << endl;
            sem_post(&more_needed);
            cout << "Smoker with match request for more." << endl;
        }
        else {
            sem_post(&tobacco_sem);
            cout << "Smoker with match drop tobacco on table." << endl;
        }
        sleep(1);
    }
}

void* smoker_paper(void *arg) {
    while (true) {
        sem_wait (&match_sem);
        cout << "Smoker with paper grab match from table." << endl;
        if (sem_trywait (&tobacco_sem) == 0) {
            cout << "Smoker with paper grab tobacco from table." << endl;
            sem_post(&more_needed);
            cout << "Smoker with paper request for more." << endl;
        }
        else {
            sem_post(&match_sem);
            cout << "Smoker with paper drop match on table." << endl;
        }
        sleep(1);
    }
}
//

// Agent thread
void* agent (void *arg) {
    while (true) {
        int number = rand() % 3;
        cout << number;
        if (number == 0) {
            sem_post(&match_sem);
            cout << "Agent drop match on table." << endl;
            sem_post(&paper_sem);
            cout << "Agent drop paper on table." << endl;
        } else if (number == 1) {
            sem_post(&match_sem);
            cout << "Agent drop match on table." << endl;
            sem_post(&tobacco_sem);
            cout << "Agent drop tobacco on table." << endl;
        } else if (number == 2) {
            sem_post(&tobacco_sem);
            cout << "Agent drop tobacco on table." << endl;
            sem_post(&paper_sem);
            cout << "Agent drop paper on table." << endl;
        }
        sem_wait (&more_needed);   
    }
}
//

void initSemaphores() {
    sem_init(&match_sem, 0, 1);
    sem_init(&paper_sem, 0, 1);
    sem_init(&tobacco_sem, 0, 1);
    sem_init(&more_needed, 0, 1);
}

pthread_t agentThread, smokerTobaccoThread, smokerMatchThread, smokerPaperThread;

void createThreads() {
    pthread_create(&smokerMatchThread, NULL, smoker_tobacco, NULL);
    pthread_create(&smokerTobaccoThread, NULL, smoker_paper, NULL);
    pthread_create(&smokerPaperThread, NULL, smoker_match, NULL);
    pthread_create(&agentThread, NULL, agent, NULL);
}

void joinThreads() {
    pthread_join(agentThread, NULL);
    pthread_join(smokerTobaccoThread, NULL);
    pthread_join(smokerMatchThread, NULL);
    pthread_join(smokerPaperThread, NULL);
}

void destroySemaphores() {
    sem_destroy(&match_sem);
    sem_destroy(&paper_sem);
    sem_destroy(&tobacco_sem);
    sem_destroy(&more_needed);
}

int main() {
    initSemaphores();
    createThreads();
    joinThreads();
    destroySemaphores();
    return 0;
}

