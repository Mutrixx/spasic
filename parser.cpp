#include <iostream>

extern "C"{
#include "spasic.h"
}
int pdf[2];

class BASIC_instruction{
public:
  int line_number;
  int opcode;
  variable *operands;
};

class variable{
public:
  int type;
  void *data;  
};
  
int main(int argc, char **argv){
  int pid;
  struct list *input_head;

  pipe(pdf);
  pid=fork();
  if(pid==0){
    close(pdf[1]);
    machine_init(pfd, argv[1]);    
  }
  else if(pid>0){
    close(pdf[0]);
    parse_loop(pdf, input_head);
  }
}

void parse_loop(){
  std::list input_list, program;
  pthread_t input_thread;
  bool stop_input=0;
  string input;
  int state;
  
  pthread_create(&input_thread, NULL, &get_input, &input_list, &stop_input);
  pthread_create(&machine_output_thread, NULL, &get_machine_output, &stop_output);
  while(1){
    input=pop_back(input_list);
    
    /*- A BASIC command is a BASIC instruction given to the parser without a line number whilst the interpreter
        awaiting commands and not code.
        Such commands will be executed immediatley and not added to the program list. */
    
    if(is_BASIC_command(input)){
      BASIC_instruction tmp_instruction;
      int rc;
      tmp_instruction=parse_instruction(input);
      rc=execute_instruction(tmp_instruction);
      continue;
    }
    else{
      
      
    }
    switch(rc){
      /*Handle errors*/
      
    }
  }
}

void *get_input(list *input_list, int *stop_input){
  string buffer;
  while(1){
    getline(cin, buffer);
    input_list.push_front(buffer);
    if(stop_input)
      break;
  }
  pthread_exit();
}

void *get_machine_output(bool stop_input){
  string buffer;
  while(1){
    getline(pfd[1], buffer);
    cout << buffer << endl;
  }
}
