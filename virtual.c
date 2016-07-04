#include "spasic.h"

int init_machine(char *argv){
  void *machine;
  int fd[2];
  long int memory_size = DEFAULT_MEMORY;

  if(argv){
    memory_size=strtol(argv, NULL, 0);
    if(errno = ERANGE || memory_size<0){
      memory_size=DEFAULT_MEMORY;
      message("WARNING: memory size out of range, defaulting to %i bytes\n", DEFAULT_MEMORY);
	}
  }
  machine = malloc(memory_size);
  if(!machine){
    int countdown = 5;
    while(countdown>=0){
      machine = malloc(sizeof(memory_size));
      sleep(1);
      if(machine)
	break;
      countdown--;
    }
    message("WARNING: Couldn't allocate desired memory size. Quitting\n");
    //tell interpreter quitting
  }
  
}

void message(char *message){
  return;
}
