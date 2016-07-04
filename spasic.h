#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#define DEFAULT_MEMORY 1024

enum OPCODES{
  NOP,
  HALT,
  NUKE,
  FETCH,
  STORE
};
