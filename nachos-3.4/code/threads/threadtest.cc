#include "copyright.h"
#include "system.h"
#include "synch.h"

// testnum is set in main.cc
int testnum = 1;
int SharedVariable;

#ifdef HW1_SEMAPHORES
Semaphore *sem = NULL;  // Declare semaphore globally, but initialize later
#endif

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void SimpleThread(int which) {
    int num, val;
    for (num = 0; num < 5; num++) {
        #ifdef HW1_SEMAPHORES
        sem->P();  // Acquire semaphore before accessing shared variable
        #endif

        val = SharedVariable;
        printf("*** thread %d sees value %d\n", which, val);
        currentThread->Yield();
        SharedVariable = val + 1;

        #ifdef HW1_SEMAPHORES
        sem->V();  // Release semaphore after updating shared variable
        #endif
        
        currentThread->Yield();
    }

    #ifdef HW1_SEMAPHORES
    sem->P();  // Acquire semaphore before reading final value
    #endif

    val = SharedVariable;
    printf("Thread %d sees final value %d\n", which, val);

    #ifdef HW1_SEMAPHORES
    sem->V();  // Release semaphore
    #endif
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine with multiple threads.
//
//	n is the number of threads to fork.
//----------------------------------------------------------------------

void ThreadTest(int n) {
    DEBUG('t', "Entering ThreadTest with %d threads\n", n);
    Thread *t;

    #ifdef HW1_SEMAPHORES
    // Initialize the semaphore with 1 to control access to SharedVariable
    sem = new Semaphore("SharedVariable Semaphore", 1);
    #endif

    // Fork 'n' threads
    for (int i = 1; i < n; i++) {
        t = new Thread("forked thread");
        t->Fork(SimpleThread, i);
    }

    // Run SimpleThread(0) on the original thread
    SimpleThread(0);
}
