#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

// structure spinlock
struct SpinLock {
    atomic<bool> locked;

    SpinLock() : locked(false) {}

    void lock() {
        while (true) {
            if (!locked.exchange(true)) {
                return; // successful lock
            }
            // wait if someone is holding the lock
            this_thread::yield();
        }
    }

    void unlock() {
        locked.store(false);
    }
};

// numebr of philosphers
int num_philos;

// spinlock array
SpinLock* forks;

// number of iterations
int iterations;

void philosopher(int phil_no) {
    int iter = 0;

    while (iter < iterations) {
		//thinking
        cout << "Filozof " << phil_no << " myśli.\n";
        this_thread::sleep_for(chrono::seconds(1)); // wait

		//eating
        cout << "Filozof " << phil_no << " próbuje jeść.\n";
        if (phil_no % 2 == 0) {
            // even number
            cout << "Filozof " << phil_no << " próbuje podnieść lewy widelec.\n";
            forks[phil_no].lock(); // get left fork
            cout << "Filozof " << phil_no << " podniósł lewy widelec.\n";
            cout << "Filozof " << phil_no << " próbuje podnieść prawy widelec.\n";
            forks[(phil_no + 1) % num_philos].lock(); // get right fork
            cout << "Filozof " << phil_no << " podniósł prawy widelec.\n";
        } else {
            // odd number
            cout << "Filozof " << phil_no << " próbuje podnieść prawy widelec.\n";
            forks[(phil_no + 1) % num_philos].lock(); // get right fork
            cout << "Filozof " << phil_no << " podniósł prawy widelec.\n";
            cout << "Filozof " << phil_no << " próbuje podnieść lewy widelec.\n";
            forks[phil_no].lock(); // get left fork
            cout << "Filozof " << phil_no << " podniósł lewy widelec.\n";
        }

        cout << "Filozof " << phil_no << " je.\n";
        this_thread::sleep_for(chrono::seconds(1)); // eat

		//put down fork
        cout << "Filozof " << phil_no << " próbuje odłożyć prawy widelec.\n";
        forks[(phil_no + 1) % num_philos].unlock(); // right fork
        cout << "Filozof " << phil_no << " odłożył prawy widelec.\n";
        cout << "Filozof " << phil_no << " próbuje odłożyć lewy widelec.\n";
        forks[phil_no].unlock(); // left fork
        cout << "Filozof " << phil_no << " odłożył lewy widelec.\n";

        iter++;
    }

    cout << "Filozof " << phil_no << " zakończył działanie.\n";
}

int main() {
	// philosophers number
    cout << "Podaj liczbę filozofów: ";
    cin >> num_philos;

	// iterations number
    cout << "Podaj liczbę iteracji: ";
    cin >> iterations;

    forks = new SpinLock[num_philos];

	// creating threads
    thread threads[num_philos];
    for (int i = 0; i < num_philos; i++) {
        threads[i] = thread(philosopher, i);
    }
	
	// thread synchronization
    for (int i = 0; i < num_philos; i++) {
        threads[i].join();
    }

    delete[] forks;

    return 0;
}
