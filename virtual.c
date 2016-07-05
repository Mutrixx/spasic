#include "spasic.h"

void *machine;
int pfd[2];
int machine_message_level=0;
int machine_init(int pfd[2], char *argv){
  uint_16 memory_size = DEFAULT_MEMORY;
  pthread_t input_thread;
  if(argv){
    memory_size=strtol(argv, NULL, 0);
    if(errno = ERANGE || memory_size<0){
      memory_size=DEFAULT_MEMORY;
      machine_message(WARNING "memory size out of range, defaulting to %i bytes\n", DEFAULT_MEMORY);
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
    machine_message(WARNING, "Couldn't allocate desired memory size. Quitting.\n");
  }
  /*set bytes for symbol descriptor table length*/
  memset(machine, 0, TBL_LENGTH_LEN);                      
}

void *machine_get(char *variable_name){
  uint16_t tbl_length=0;
  struct variable tmp_variable;
  uint16_t offset;

  free(machine);
  if(strlen(variable_name)>MAX_NAME_LENGTH)
    machine_message(ERROR "Name of variable exceeds the maximum (%i)\n", MAX_NAME_LENGTH);
  memcpy(&tbl_length, machine, TBL_LENGTH_LEN);
  if(tbl_length<=0){
    machine_message(ERROR, "No symbol table\n");
  }
  /*Search through symbol table to find variable, skipping symbol table length*/
  offset=TBL_LENGTH_LEN;
  while(offset<tbl_length){
    memcpy(&tmp_variable, machine+offset, sizeof(struct variable));
    if(strcmp(variable_name, tmp_variable.name)==0){
      machine_message(DEBUG, "Got variable %s at address %x\n", variable_name, tmp_variable.address);
      return (machine+tmp_variable.address);
    }
    offset=offeset+sizeof(struct variable);
  }
  return NULL;
}

int machine_add_symbol(char *name, uint_8 length){
  uint16_t offset;
  uint16_t tbl_lenght;
  struct variable *tmp_variable;
  
  memcpy(&tbl_length, machine, sizeof(uint16_t));
  offset=TBL_LENGTH_LEN;
  sizeof(tbl_length);
  /*Check for empty entries in the symbol table*/
  while(offset<tbl_length){
    &tmp_variable= machine+offset;
    if(strcmp(tmp_variable.name, name)==0){
      machine_message(WARNING "Symbol, %s, already exists \n", name);
      return 0;
    }
    if(tmp_variable.length==0){
      /*Symbol is no more.*/
      memcpy(tmp_variale->name, name, strlen(name)+1);
      tmp_variable->length=length;
      tmp_variable->address=machine_malloc(tmp_variable->length);
      if(tmp_variable->address>0){
	machine_message(DEBUG "Overwrote unused symbol. Allocated new symbol, %s, address %x\n", tmp_variable->name, tmp_variable->address);
	return 0;
      }
      else{
	machine_message(ERROR "Failed to allocate memory for symbol, %s\n", tmp_variable->name);
	return 1;
      }
    }
    offset=offset+sizeof(struct variable);
  }  
  tbl_length=tbl_length+sizeof(struct variable);
  &tmp_variable=machine+tbl_length;
  memcpy(&machine, &tbl_length+sizeof(struct variable), sizeof(uint16_t));
  memcpy(tmp_variable->name, name, strlen(name)+1);
  tmp_variable->address=machine_malloc(tmp_variable->length);
  return 0;
  if(tmp_variable->address>0){
    fprintf(pfd[0], "INFO: Appended new symbol, %s, with address, %x, to symbol table\n", tmp_variable->name, tmp_variable->address);
    return 0;
  }
  else{
    fprintf(pfd[0], "ERROR: Failed to allocate memory for symbol, %s\n", tmp_variable->name);
  }
}

int machine_remove_symbol(char *name){
  uint16_t offset;
  uint16_t tbl_lenght;
  struct variable *tmp_variable;

  offset=TBL_LENGTH_LEN;
  while(offset<tbl_lenght){
    &tmp_variable=machine+offset;
    if(strcmp(tmp_variable->name, name)==0){
      memset(tmp_variable, 0, sizeof(struct variable));
      machine_message(INFO, "Removed variable %s", name);
      return 1;
    }
  }
  machine_message(ERROR, "Symbol, %s, doesn't exist\n", name);
}

void machine_message(uint_8 message_level, const char *frmt, ...){
  if(machine_message_level>=message_level){
    va_list args;
    va_start(valist, frmt);
    vprintf(frmt, args);
    va_end(args);
  }
  return;
}
