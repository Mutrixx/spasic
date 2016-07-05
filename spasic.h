#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_NAME_LENGTH 16
#define DEFAULT_MEMORY 1024

enum data_type{
  INT,
  CHAR,
  FLOAT,
  BOOL
};

struct variable{
  short int length;
  int address;
  char name[MAX_NAME_LENGTH];
};

struct data{
  void *payload;
  short int length;
}
