#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdint.h>
#include <stdarg.h>

#define MAX_NAME_LENGTH 16
#define DEFAULT_MEMORY 1024
#define TBL_LENGTH_LEN 4
enum data_type{
  INT,
  CHAR,
  FLOAT,
  BOOL
};

struct symbol{
  uint_8 length;
  uint_16 address;
  const char name[MAX_NAME_LENGTH];
};

struct value{
  int type;
  void *value;
}

enum MESSAGE_LEVELS{
  CRITICAL,
  ERROR,
  WARNING,
  INFO,
  DEBUG
};

enum STATES{
  NONE,
  MACRO,
  QUIT
};

enum INSTRUCTIONS{
  GOTO,
  PRINT,
  FOR,
  WHILE,
  WEND,
  NEXT,
  LET
};


