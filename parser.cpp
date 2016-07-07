#include <iostream>

extern "C"{
#include "spasic.h"
}

int pdf[2];
int pid;

class BASIC{
public:
  BASIC(string input);
  void print();
  int line_number;
  int operation;
  std::list<string> operands;
  BASIC& operator=(BASIC_instruction &other); 
};

void BASIC::print(){
  std::list<string>::iterator j;
  std::cout << line_number;
  std::cout << operation_to_string(operation);
  for(j=0;j<program.operands.size();j++)
    std::cout << operands[j];
  std::count << endl;
}

class variable{
public:
  int type;
  void *data;  
};

struct subexpression{
  string token[2];
  char   operator;
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
	else if(tmp_instruction.operation=="FOR"||tmp_instruction.operation=="IF"||tmp_instruction.operation="DATA"||tmp_instruction.operation="WHILE"||tmp_instruction.operation=="DO"){
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
	      program[i].print();
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
  int i;
  for(i=0;i<program.size();i++){
    if(tron)
      cout << program[i].line_number << endl;
    if(operation==IF||operation==WHILE){
      struct value value;

      value=evaluate(program.operands);
      if(value.type!=BOOL){
	message(ERROR, "Condition is not a boolean expression.\n");
	return;
      }
      if(*(value.value)==TRUE)
	continue;
      if(*(value.value)==FALSE){
	int j;
	int depth=1;

	if(program.operation==WHILE){
	  for(j=i;j>=0;j--;){
	    if(program[j].operation==DO)
	      depth=depth-1;
	    if(program[j].operation==WHILE)
	      depth=depth+1;
	    if(program[j].operation==WEND)
	      depth=depth+1;
	    if(depth==0){
	      if(program[j].operation!=DO){
		message(ERROR, "Malformed DO-WHILE loop.\n");
		return;
	      }
	      else{
		i=j;
		continue;
	      }
	    }
	  }
	  for(j=i;j<program.size();j++){
	    if(((program[j].operation==ENDIF)&&(program[i].operation==IF))
	       || (program[j].operation==WEND)&&(program[i].operation==WHILE))
	      depth=depth-1;
	    if(program[j].operation==program[i].operation)
	      depth=dept+1;
	    if(depth==0)
	      i=j;
	  }
	  message(WARNING, "IF statement doesn't have corresponding 
                          ENDIF instruction \n");
	}
      }
      else if(program[i].operation==PRINT){
	struct value value;

	if(is_variable(operation.operands)){
	  struct symbol *symbol;
	  char *print;
	  symbol=machine_get_symbol(operation.operands.c_str());
	  print = malloc(symbol->length+1);
	  memcpy(print, symbol->address + machine, symbol->length);
	  print[symbol->length]='\0';
	  printf("%s\n", print);
	}
	value=evaluate(program[i].operands);      
      }
      else if(program[i].operation==GOTO){
	int line,j;
	line=atoi(program[i].operands);
	for(j=0;j<program.size();j++){
	  if(program[j].line_number==line){
	    i=j-1;
	    continue;
	  }
	  else if(program[j].line_number<line){
	    i=j-1;
	    message(WARNING "Line %i doesn't exist. Continuing execution from
                           line %i.\n", line, j);
	    continue;
	  }
	}
	message(ERROR, "Line %i doesn't exist. No further lines exist. Stopping execution of program.\n") ;
	return;
      }
    }
    message(INFO, "Program has finished execution.\n");
    return;
  }
}

struct value evaluate(string expression){
  std::vector operations;
  std::vector tokens;
  std::list<subexpression> subexpressions;
  
  int i;
  int last_operation=0;
  
  for(i=0;i<expression.size();i++){
    switch(expression[i]){
    default:
      break;
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
    case '^':
      tokens.push_back(expression[i]);
      goto STORE_TOKEN;
      break;
    case '<':
      if(i+1<expression.size()){
	if(expression[i+1]=='='){
	  operations.push_back('l');
	  break;
	}
      }
      operations.push_back('<');
      goto STORE_TOKEN;
    case '>':
      if(i+1<expression.size()){
	if(expression[i+1]=='='){
	  operations.push_back('g');
	  break;
	}
      }
      operations.push_back('>');
      goto STORE_TOKEN;
      break;
    }
  STORE_TOKEN:
  tokens.push_back(substr(last_operation, i-last_operation));
  if(tokens.back().empty()){
    message(ERROR, "Malformed expression.\n");
    return;
  }
  }
    for(i=0;i<operations.length();i++){
    subexpression tmp_expression;
    subexpression.token[0]=tokens[2*i];
    subexpression.token[1]=tokens[2*i+1];
    switch(operations[i]){
  case '^':
    subexpression.operation=INDEX;
    break;
  case '/':
    subexpression.operation=DIVIDE;
    break;
  case '*':
    subexpression.operation=MULTIPLY;
    break;
  case '+':
    subexpression.operation=PLUS;
    break;
  case '-':
    subexpression.operation=MINUS;
    break;
  case '>':
    subexpression.operation=GREAT;
    break;
  case 'g':
    subexpression.operation=GREAT_E;
    break;
  case '<':
    subexpression.operation=LESS;
    break;
  case 'l':
    subexpression.operation=LESS_E;
    break;
  }
    
  }
  }
