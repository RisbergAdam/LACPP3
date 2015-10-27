#include <stdio.h>

int main() {
  int a = 0;

  try {
    __transaction_atomic {
      a = 1;
      throw 1337;
    }
  } catch (int e) {

  }

  //if a is 1, the transaction commited despite a thrown exception
  printf("a: %i\n", a);

  return 0;
}
