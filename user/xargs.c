#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Argument required!\n");
    exit(-1);
  }

  char* command = argv[1];
  if (fork() == 0) {
    printf("child running %s\n", command);
    exec(command, argv + 1);
  } else wait(0);
  const int maxLength = 512;
  char line[maxLength];
  const int stdin = 0;
  int offset = 0;
  char c = 0;
  char* myArgs[3];
  myArgs[0] = command;
  myArgs[1] = line;
  myArgs[2] = 0;
  // printf("while loop about to run\n");
  while (read(stdin, &c, 1)) {
    // printf("%c", c);
    if (c == '\n') {
      line[offset] = 0;
      if (fork() == 0) {
        // printf("loop child about to run %s %s", myArgs[0], myArgs[1]);
        exec(command, myArgs);
        // exec(command, argv);
      } else {
        wait(0);
        offset = 0;
      }
    } else {
      line[offset++] = c;
    }
  } if (offset != 0) {
    line[offset] = 0;
    if (fork() == 0) {
      line[offset] = 0;
      exec(command, myArgs);
    } else {
      wait(0);
      offset = 0;
    }
  }

  exit(0);
}
