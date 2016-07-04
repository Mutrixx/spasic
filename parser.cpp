#include <iostream>

extern "C"{
#include "spasic.h"
}

int main(int argc, char **argv){
  int pid;
  struct list *input_head;
  pid=fork();
  if(pid==0){
    init_machine(argv[1]);    
  }
  else if(pid>0){
    get_input(input_head);
  }
  init_machine(argv[1]);
}
