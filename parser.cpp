#include <iostream>

extern "C"{
#include "spasic.h"
}

int pdf[2];
int pid;

class BASIC{
public:
  BASIC(string input);
  int line_number;
  int opcode;
  std::list operands;
  BASIC& operator=(BASIC_instruction &other); 
};

class variable{
public:
  int type;
  void *data;  
};

BASIC& operator=(BASIC other){
  using std::swap;
  swap(line_number, other.line_number);
  swap(opcode, other.opcode);
  free(operands);
  operands=other.operands;
   
}
  
int main(int argc, char **argv){
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
  std::list input_list;
  std::list<BASIC> program;
  pthread_t input_thread;
  bool stop_input=0;
  string input;
  int state;
  bool tron = 0;
  BASIC tmp_instruction;
  
  pthread_create(&input_thread, NULL, &get_input, &input_list, &stop_input);
  pthread_create(&machine_output_thread, NULL, &get_machine_output, &stop_output);
  while(1){
    input=input_list.back
    tmp_instruction = BASIC(input);
    
    /*- A BASIC command is a BASIC instruction given to the parser without a line number whilst the interpreter is awaiting commands and not code.
        Such commands will be executed immediatley and not added to the program list. */
    
    if(tmp_instruction.opcode>0){
      int rc;
      if(tmp_instruction.line_number>=0){
	std::list<BASIC>::iterator i;
	for(i=0;i<program.size();i++){
	  if(program[i].line_number==tmp_instruction.line_number){
	    message(INFO, "Line %i of the program already exists. Replacing\n");
	    program[i]=tmp_instruction;
	  }
	  else if(program[i].line_number>tmp_instruction.line_number){
	    program.insert(i, tmp_instruction);
	    break;
	  }
	}
	if(tmp_instruction.line_number>program[list.size()-1])
	  program.push_back(tmp_instruction);
      }
      else
	rc=execute_instruction(tmp_instruction);
    else{
      if(input=="RUN"){

	
      }
      else if(input=="TRON")
	tron=TRUE;
      else if(input=="TROFF")
	tron=FALSE;
      else if(input=="QUIT")
	state=EXIT;
      else if(input=="RUN")
	
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
