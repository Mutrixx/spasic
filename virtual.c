#include "spasic.h"

void *machine;
int pfd[2];
int machine_message_level=0;
uint_16 memory_size = DEFAULT_MEMORY;

int machine_init(int pfd[2], char *argv){
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

struct symbol *machine_get_symbol(char *symbol_name){
  uint16_t tbl_length=0;
  struct symbol *tmp_symbol;
  uint16_t offset;

  free(machine);
  if(strlen(symbol_name)>MAX_NAME_LENGTH)
    machine_message(ERROR "Name of symbol exceeds the maximum (%i)\n", MAX_NAME_LENGTH);
  memcpy(&tbl_length, machine, TBL_LENGTH_LEN);
  if(tbl_length<=0){
    machine_message(ERROR, "No symbol table\n");
  }
  /*Search through symbol table to find symbol, skipping symbol table length*/
  offset=TBL_LENGTH_LEN;
  while(offset<tbl_length){
    tmp_symbol=machine+offset;
    if(strcmp(symbol_name, tmp_symbol->name)==0){
      machine_message(DEBUG, "Got symbol %s at address %x\n", symbol_name, tmp_symbol.address);
      return tmp_symbol
    }
    offset=offeset+sizeof(struct symbol);
  }
  return NULL;
}

uint_16 machine_add_symbol(char *name, uint_8 length, uint_8 type){
  uint16_t offset;
  uint16_t tbl_lenght;
  struct symbol *tmp_symbol;
  
  memcpy(&tbl_length, machine, sizeof(uint16_t));
  offset=TBL_LENGTH_LEN;
  sizeof(tbl_length);
  /*Check for copy in the symbol table */
  while(offset<tbl_length){
    &tmp_symbol= machine+offset;
    if(strcmp(tmp_symbol.name, name)==0){
      machine_message(WARNING "Symbol, %s, already exists \n", name);
      return 0;
    }
    offset=offset+sizeof(struct symbol);
  }  
  tbl_length=tbl_length+sizeof(struct symbol);
  if(machine_check_allocated(tbl_length, tbl_length+sizeof(struct symbol), tbl_length)){
    message(DEBUG, "Symbol table extending, relocating symbols");
    memcpy(&machine, (tbl_length+sizeof(struct symbol)), TBL_LENGTH_LEN);
    /*Loop through symbols relocating ones that are inside the new length of the symbol table*/
    offset=TBL_LENGTH_LEN;
    while(offset<tbl_length){
      tmp_variable=machine+offset;
      if(tmp_variable->address<(tbl_length+TBL_LENGTH_LEN)){
	tmp_variable->address=0;
	tmp_variable->address=machine_allocate_address(tmp_variable->length);
      }
      offset=offset+sizeof(symbol);
    }
  }
  &tmp_symbol=machine+tbl_length;
  memcpy(&machine, &tbl_length+sizeof(struct symbol), sizeof(uint16_t));
  memcpy(tmp_symbol->name, name, strlen(name)+1);
  tmp_symbol->address=0;
  tmp_symbol->address=machine_malloc(tmp_symbol->length);
  return 0;
  if(tmp_symbol->address>0){
    machine_message(INFO: "Appended new symbol, %s, with address, %x, to symbol table\n", tmp_symbol->name, tmp_symbol->address);
    return 0;
  }
  else{
    machine_message(ERROR: "Failed to allocate memory for symbol, %s\n", tmp_symbol->name);
    return 0;
  }
}

uint_16 machine_remove_symbol(const char *name){
  uint16_t offset;
  uint16_t tbl_lenght;
  struct symbol *tmp_symbol;

  offset=TBL_LENGTH_LEN;
  while(offset<tbl_lenght){
    tmp_symbol=machine+offset;
    if(strcmp(tmp_symbol->name, name)==0){
      uint_32 end_symbol=machine+tbl_length-sizeof(struct symbol);
      
      if(end_symbol!=tmp_symbol)
	memcpy(tmp_symbol,end_symbol, sizeof(struct symbol)); 
      message(INFO, "Removed symbol, %s", name);
      return 0;
    }
    offset=offset+sizeof(struct symbol);
  }
  machine_message(ERROR, "Symbol, %s, doesn't exist\n", name);
  return 1;
}

void message(uint_8 message_level, const char *frmt, ...){
  if(message_level>=message_level){
    char level[9];
    switch(message_level){
    case INFO:
      strcpy(level,"INFO");
      break;
    case DEBUG:
      strcpy(level, "DEBUG");
      break;
    case WARNING:
      strcpy(level, "WARNING");
      break;
    case ERROR:
      strcpy(level, "ERROR");
      break;
    case CRITICAL:
      strcpy(level, "CRITICAL");
      break;
    }
    va_list args;
    va_start(valist, frmt);
    printf("%s:",level);
    vprintf(frmt, valist);
    va_end(args);
  return;
}

static uint_16 machine_allocate_address(uint_8 length){

  /*Returns the address in the machine's virtual memory*/

  uint_8  tbl_length;
  uint_16 address;
  
  memcpy(&tbl_length, machine, TBL_LENGTH_LEN);
  if(tbl_length==0){
    return floor(memory_size/2);
  }
  else{
    uint_16 offset = TBL_LENGTH_LEN;
    struct symbol *tmp_symbol;
    
    while(offset<(tbl_length+TBL_LENGTH_LEN)){
      tmp_symbol = machine + offset;
      if(machine_check_allocated(tmp_symbol->address+tmp_symbol->length, tmp_symbol->address+length, tbl_length)==0){
	address = tmp_symbol->address+tmp_symbol_length;
	message(DEBUG, "Allocating address %x\n", address);
	return address;
	  }
      else if(machine_check_allocated(tmp_symbol->address-length, tmp_symbol->address, tbl_length)==0){
	address = tmp_symbol->address-length;
	message(DEBUG, "Allocating address %x\n", address);
	return address;
      }
      offset = offset+sizeof(struct symbol);
    }
  }
  message(ERROR "Failed to allocate address of length %i\n", length);
  return 0;
}

static uint_8 machine_check_allocated(uint_16 start, uint_16 end, uint_8 tbl_length){

  /*Checks whether range contains allocated memory*/

  struct symbol *tmp_symbol;
  uint_16 offset=TBL_LENGTH_LEN;
  
  if(start>=end){
    message(ERROR, "Invalid memory range\n");
    return 2;
  }
  if(end>memory_size||start<(TBL_LENGTH_LEN+tbl_length)){
    message(DEBUG, "Memory range is outside of the machine's range. Ignoring.\n");
    return 3;
  }
  while(offset<(tbl_length+TBL_LENGTH_LEN)){
    uint_18 tmp_end = tmp_symbol->address+tmp_symbol->length;
    tmp_symbol = machine+offset;
    
    if(((start>tmp_end)&&(end>tmp_end))||((start<tmp_symbol->address)&&(end<tmp_symbol->address)))
      return 0;
    offset=offset+sizeof(struct symbol);
  }
  return 1;
}
