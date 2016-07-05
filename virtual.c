#include "spasic.h"

int machine_init(int pfd[2], char *argv){
  void *machine;
  long int memory_size = DEFAULT_MEMORY;
  pthread_t input_thread;
  if(argv){
    memory_size=strtol(argv, NULL, 0);
    if(errno = ERANGE || memory_size<0){
      memory_size=DEFAULT_MEMORY;
      fprintf(pfd[0], "WARNING: memory size out of range, defaulting to %i bytes\n", DEFAULT_MEMORY);
	}
  }
  machine = malloc(memory_size);
  if(!machine){
    int countdown = 5;
    while(countdown>0){
      machine = malloc(sizeof(memory_size));
      sleep(1);
      if(machine)
	break;
      countdown--;
    }
    fprintf(pfd[0],"WARNING: Couldn't allocate desired memory size. Quitting\n");
  }
  memset(machine, 0, 4);                      /*set bytes for symbol descriptor table length*/
}

struct data machine_get(char *variable_name){
  long int tbl_length=0;
  struct variable tmp_variable;
  struct data data;

  if(strlen(variable_name)>MAX_NAME_LENGTH)
    fprintf(pfd[0], "ERROR: Name of variable exceeds the maximum (%i)\n", MAX_NAME_LENGTH);
  memcpy(&tbl_length, machine, 4);
  if(tbl_length<=0){
    fprintf(pfd[0], "ERROR: No symbol table\n");
  }
  /*Search through symbol table to find variable, skipping symbol table length*/
  offset=4;
  while(offset<tbl_length){
    memcpy(&tmp_variable, machine+offset, sizeof(struct variable));
    if(strcmp(variable_name, tmp_variable.name)==0){
      data.length=tmp_variable.length;
      data.payload=tmp_variable.address+machine;
      return data;
    }
    offset=offeset+sizeof(struct variable);
  }
  data.length=0;
  data.payload=NULL;
  return data;
}
