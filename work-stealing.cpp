#include <atomic>
#include <iostream>
#include <random>
#include <thread>
#include <unistd.h>
#include <cstdlib>

std::atomic<bool> terminate;

////////////////////////////////////////////////////////////////////////////////

#include "deque.cpp"

const int N = 4; // number of processors

DQueue job_queue[N];



int getRandomProcessor(int me)
{
  int stealFrom = rand() % N;
  if (stealFrom == me) return getRandomProcessor(me);
  return stealFrom;
}

int steal(int me)
{
  int stealFrom = getRandomProcessor(me);
  __transaction_relaxed {
    int stolen = job_queue[stealFrom].PopRight();
    if (stolen == 0) {
      job_queue[stealFrom].PushRight(0);
      return 0;
    }
    else if (stolen == -1) return 0;
    else {
      printf("Process %i is stealing from %i with a workload %i\n", me,stealFrom,stolen);
      return stolen;
    }
  }
}

void processor(int n)
{
  while (!terminate)
  {
    // TODO
    int poped = job_queue[n].PopLeft();
    if (poped >= 0) {
      printf("%i poped: %i\n",n, poped);
      sleep(poped);
    } else {
      int stolen = steal(n);
      if(stolen > 0) printf("%i sleeping %i (stolen)\n", n, stolen);
      sleep(stolen);
    }


  }
}


////////////////////////////////////////////////////////////////////////////////

void user()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> processor(0, N - 1);
  std::uniform_int_distribution<int> duration(0, 2 * N);

  unsigned int time = 0;

  // generates a new job about once per second
  while (!terminate)
  {
    int p = processor(gen);
    int d = duration(gen);

    __transaction_relaxed
    {
      std::cout << time << ": scheduling a job on processor " << p << " (duration: " << d << " s)." << std::endl;
      job_queue[p].PushRight(d);
    }

    sleep(1);
    ++time;
  }
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
  std::thread user_thread(user);

  std::thread processors[N];
  for (int i = 0; i < N; ++i)
  {
    processors[i] = std::thread(processor, i);
  }

  sleep(60);
  terminate = true;

  user_thread.join();
  for (int i = 0; i < N; ++i)
  {
    processors[i].join();
  }

  return 0;
}
