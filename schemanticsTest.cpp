#include <stdio.h>


int main() {
  int i = 0;

  __transaction_atomic {
    
    __transaction_atomic {
      //if flattened, this will cancel outer transaction
      __transaction_cancel;
    }

    //if closed, this transaction will not commit before the next if-statement
    __transaction_atomic {
      i = 3;
    }

    if (i == 3) {
      //prevoius transaction commited and visible = open schemantics
      i = 1;
    } else {
      //i still 0, previous transaction still not comitted, closed schemantics
      i = 2;
    }
    
  }

  printf("%i\n", i);

  //i == 0: flattened
  //i == 1: open
  //i == 2: closed
  
  return 0;
}
