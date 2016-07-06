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
  string operation;
  std::list<string> operands;
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
  swap(operation, other.operation);
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
  std::list<BASIC> program, macro;
  pthread_t input_thread;
  bool stop_input=0;
  string input;
  int state=NONE;
  bool tron = 0;
  BASIC tmp_instruction;
  
  pthread_create(&input_thread, NULL, &get_input, &input_list, &stop_input);
  pthread_create(&machine_output_thread, NULL, &get_machine_output, &stop_output);
  while(1){
    input=input_list.back;
    if(!input)
      continue;
    tmp_instruction = BASIC(input);
    
    /*- A BASIC command is a BASIC instruction given to the parser without a line number whilst the interpreter is awaiting commands and not code.
        Such commands will be executed immediatley and not added to the program list. */
    if(state==QUIT){


    }
    if(state==MACRO){
      if(tmp_instruction.operation<0){
	message(ERROR, "%s is not a valid BASIC instruction", input);
	state=NONE;
      }
      if(tmp_instruction.line_number>=0){
	message(WARNING "You are in the process of entering a one-time BASIC instruction,
                           line numbers will be ignored.\n");
      }
      else{
	macro.push_back(tmp_instruction);
      }
      if(macro_finished(macro)){
	run(macro, 0, 0);
      }	
    }
    else if(state==NONE){
      if(tmp_instruction.operation>0){
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
	else if(tmp_instruction.operation=="FOR"||tmp_instruction.operation=="IF"||tmp_instruction.opcode=DATA||tmp_instruction.opcode=WHILE||tmp_instruction.opcode==DO){
	  state=MACRO;
	  macro.empty();
	  macro.push_front(tmp_instruction);
	}
	else
	  rc=execute_instruction(tmp_instruction);
	else{
	  if(input=="RUN"){
	    run(program,tron,0);	
	  }
	  else if(input=="TRON")
	    tron=TRUE;
	  else if(input=="TROFF")
	    tron=FALSE;
	  else if(input=="QUIT")
	    state=EXIT;
	  else if(input=="RUN")
	    execute_instruction();
	  else if(input=="LIST"){
	    std::list<BASIC>::iterator i;
	    for(i=0;i<program.size();i++){
	      std::list<string>::iterator j;
	      std::cout << program[i].line_number;
	      std::cout << opcode_to_sring(program[i].opcode);
	      for(j=0;j<program.operands.size();j++)
		std::cout << program.operands[j];
	      std::count << endl;
	    }
	  }	
	}
	switch(rc){

	  /*Handle errors*/
	  
	}
      }
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

void run(std::list<BASIC> program, bool tron, int startpoint){
  std::list<machine_code> machine_code;
  struct machine_code *machine_code;
  
  machine_code_list=compile(program);
  machine_code=code_list_array(machine_code_list);
  machine_execute(machine_code);
  return;
}

std::list<machine_code> compile(std::list program){
  int i;
  std::list<machine_code> machine_code;

  for(i=0;i<program.size();i++){
    switch(program[i].opcode){
    case: 
      
    }
  }  
}
