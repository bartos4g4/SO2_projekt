#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

// numebr of philosphers
int num_philos;

sem_t* forks;

// number of iterations
int iterations;

void* philosopher(void* arg) {
    // get philosopher number
    int phil_no = *(int*)arg;
    int iter = 0;

    while (iter < iterations) {
        // thinking
        cout << "Filozof " << phil_no << " myśli.\n";
        sleep(1); //wait

        // eating
        cout << "Filozof " << phil_no << " próbuje jeść.\n";
        if (phil_no % 2 == 0) {
            // even number
            cout << "Filozof " << phil_no << " próbuje podnieść lewy widelec.\n";
            sem_wait(&forks[phil_no]); // get left fork
            cout << "Filozof " << phil_no << " podniósł lewy widelec.\n";
            cout << "Filozof " << phil_no << " próbuje podnieść prawy widelec.\n";
            sem_wait(&forks[(phil_no + 1) % num_philos]); // get right fork
            cout << "Filozof " << phil_no << " podniósł prawy widelec.\n";
        } else {
            // odd number
            cout << "Filozof " << phil_no << " próbuje podnieść prawy widelec.\n";
            sem_wait(&forks[(phil_no + 1) % num_philos]); // get right fork
            cout << "Filozof " << phil_no << " podniósł prawy widelec.\n";
            cout << "Filozof " << phil_no << " próbuje podnieść lewy widelec.\n";
            sem_wait(&forks[phil_no]); // get left fork
            cout << "Filozof " << phil_no << " podniósł lewy widelec.\n";
        }

        cout << "Filozof " << phil_no << " je.\n";
        sleep(1); // eat

        //put down fork
        cout << "Filozof " << phil_no << " próbuje odłożyć prawy widelec.\n";
        sem_post(&forks[(phil_no + 1) % num_philos]); // right fork
        cout << "Filozof " << phil_no << " odłożył prawy widelec.\n";
        cout << "Filozof " << phil_no << " próbuje odłożyć lewy widelec.\n";
        sem_post(&forks[phil_no]); // left fork
        cout << "Filozof " << phil_no << " odłożył lewy widelec.\n";

        iter++;
    }

    cout << "Filozof " << phil_no << " zakończył działanie.\n";
    return NULL;
}

int main() {
    // philosophers number
    cout << "Podaj liczbę filozofów: ";
    cin >> num_philos;

    // iterations number
    cout << "Podaj liczbę iteracji: ";
    cin >> iterations;

    forks = new sem_t[num_philos];

    // semaphores initialization
    for (int i = 0; i < num_philos; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // threads array
    pthread_t threads[num_philos];
    int phil_no[num_philos]; // philospphers number array

    // creating threads
    for (int i = 0; i < num_philos; i++) {
        phil_no[i] = i; // assign philo number
        pthread_create(&threads[i], NULL, philosopher, &phil_no[i]); // create thread
    }

    // waiting for threads to finish
    for (int i = 0; i < num_philos; i++) {
        pthread_join(threads[i], NULL);
    }

    // delete semaphores
    for (int i = 0; i < num_philos; i++) {
        sem_destroy(&forks[i]);
    }
    // release memory
    delete[] forks; 

    return 0;
}
