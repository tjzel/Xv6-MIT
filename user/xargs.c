#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Argument required!\n");
    exit(-1);
  }

  char* command = argv[1];

  const int maxLength = 512;
  char line[maxLength];

  int myArgc = 1;
  char* myArgv[MAXARG - 1];
  for (int i = 0; i < MAXARG - 1; ++i) {
    myArgv[i] = malloc(128);
    myArgv[i][0] = 0;
  }

  for (; myArgc < argc; ++myArgc) {
    strcpy(myArgv[myArgc - 1], argv[myArgc]);
  }

  const int stdin = 0;
  int length = 0;
  char c = 0;

  while (read(stdin, &c, 1) && myArgc < MAXARG - 1) {
    if (c == '\n') {
      line[length] = 0;
      strcpy(myArgv[myArgc++], line);
      length = 0;
    } else {
      line[length++] = c;
    }
  } if (length != 0) {
    line[length] = 0;
    strcpy(myArgv[myArgc++], line);
  }
  printf("%d", exec(command, myArgv));
  exit(-1);
}
