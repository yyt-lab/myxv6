#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  if(argc == 1 || argc > 3) fprintf(2,"forget to pass argvs!\n");
  else {
      i = atoi(argv[1]);
      sleep(i);
  }
  exit(0);
}
