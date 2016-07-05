#include <iostream>

extern "C"{
#include "spasic.h"
}

int main(int argc, char **argv){
  int pid;
  int pdf[2];
  struct list *input_head;

  pipe(pdf);
  pid=fork();
  if(pid==0){
    close(pdf[1]);
    machine_init(pfd, argv[1]);    
  }
  else if(pid>0){
    close(pdf[0]);
    init_parser(pdf, input_head);
  }
  init_machine(argv[1]);
}
