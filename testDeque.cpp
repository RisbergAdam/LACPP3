#include <thread>
#include "deque.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>

DQueue * dequeue = new DQueue();
std::atomic<bool> terminate;
std::atomic<int> threadedOpCount;

void threadedPushLeft() {
  int opCount = 0;
  while (!terminate) {
    dequeue->PushLeft(1337);
    opCount++;
  }
  threadedOpCount += opCount;
}

void threadedPushPopLeft() {
  int opCount = 0;
  while (!terminate) {
    dequeue->PushLeft(1337);
    dequeue->PopLeft();
    opCount++;
  }
  threadedOpCount += opCount;
}

void threadedPushLeftPopRight() {
  int opCount = 0;
  while (!terminate) {
    dequeue->PushLeft(1337);
    dequeue->PopRight();
    opCount++;
  }
  threadedOpCount += opCount;
}

int main(int argc, char * argv []) {
  int THREAD_COUNT = atoi(argv[1]);

  std::thread threads[THREAD_COUNT];

  for (int i = 0;i < THREAD_COUNT;i++) {
    //threads[i] = std::thread(threadedPushLeft); //1: 8 200 000 2: 3 100 000
    //threads[i] = std::thread(threadedPushPopLeft); //1: 5 100 000 2: 1 000 000
    threads[i] = std::thread(threadedPushLeftPopRight);
  }

  sleep(3);

  terminate = true;

  for (int i = 0;i < THREAD_COUNT;i++) {
    threads[i].join();
  }

  printf("OpCount: %i\n", threadedOpCount.load()/3);
  
  return 0;
}
