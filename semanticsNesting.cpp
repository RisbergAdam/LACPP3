#include <stdio.h>


int main() {
  int i = 0, j = 0;

  __transaction_atomic {
    
    __transaction_atomic {
      //if flattened, this will cancel outer transaction
      __transaction_cancel;
    }

    i = 1;
  }

  __transaction_atomic {
    __transaction_atomic {
      j = 1;
    }

    __transaction_cancel;
  }

  printf("i: %i, j: %i\n", i, j);

  //i == 0: flat semantics
  //i == 1 && j == 0: closed semantics
  //i == 1 && j == 1: open semantics
  
  return 0;
}
