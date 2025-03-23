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

// number of philosophers
int num_philos;

// spinlock array
SpinLock* forks;

// number of iterations
int iterations;

void philosopher(int phil_no) {
    int iter = 0;

    while (iter < iterations) {
        //thinking
        cout << "Philosopher " << phil_no << " is thinking.\n";
        this_thread::sleep_for(chrono::seconds(1)); // wait

        //eating
        cout << "Philosopher " << phil_no << " is trying to eat.\n";
        if (phil_no % 2 == 0) {
            // even number
            cout << "Philosopher " << phil_no << " is trying to pick up the left fork.\n";
            forks[phil_no].lock(); // get left fork
            cout << "Philosopher " << phil_no << " picked up the left fork.\n";
            cout << "Philosopher " << phil_no << " is trying to pick up the right fork.\n";
            forks[(phil_no + 1) % num_philos].lock(); // get right fork
            cout << "Philosopher " << phil_no << " picked up the right fork.\n";
        } else {
            // odd number
            cout << "Philosopher " << phil_no << " is trying to pick up the right fork.\n";
            forks[(phil_no + 1) % num_philos].lock(); // get right fork
            cout << "Philosopher " << phil_no << " picked up the right fork.\n";
            cout << "Philosopher " << phil_no << " is trying to pick up the left fork.\n";
            forks[phil_no].lock(); // get left fork
            cout << "Philosopher " << phil_no << " picked up the left fork.\n";
        }

        cout << "Philosopher " << phil_no << " is eating.\n";
        this_thread::sleep_for(chrono::seconds(1)); // eat

        //put down fork
        cout << "Philosopher " << phil_no << " is trying to put down the right fork.\n";
        forks[(phil_no + 1) % num_philos].unlock(); // right fork
        cout << "Philosopher " << phil_no << " put down the right fork.\n";
        cout << "Philosopher " << phil_no << " is trying to put down the left fork.\n";
        forks[phil_no].unlock(); // left fork
        cout << "Philosopher " << phil_no << " put down the left fork.\n";

        iter++;
    }

    cout << "Philosopher " << phil_no << " has finished.\n";
}

int main() {
    do {
        // philosophers number
        cout << "Enter the number of philosophers (minimum 2): ";
        cin >> num_philos;
    
        // iterations number
        cout << "Enter the number of iterations (minimum 1): ";
        cin >> iterations;
    } while (num_philos < 2 || iterations < 1);

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
