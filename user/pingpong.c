#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int firstPipe[2];
  int secondPipe[2];

  pipe(firstPipe);
  pipe(secondPipe);

  const int pid = fork();
  const int myPid = getpid();

  if (pid == 0)
  {
    const int readEnd = firstPipe[0];
    const int writeEnd = secondPipe[1];
    char buf[5];

    close(firstPipe[1]);
    close(secondPipe[0]);

    read(readEnd, buf, 4);
    close(readEnd);

    printf("%d: received %s\n", myPid, buf);

    strcpy(buf, "pong");
    write(writeEnd, buf, 4);
    close(writeEnd);
  }
  else
  {
    const int readEnd = secondPipe[0];
    const int writeEnd = firstPipe[1];
    char buf[5] = "ping";

    close(firstPipe[0]);
    close(secondPipe[1]);

    strcpy(buf, "ping");
    write(writeEnd, buf, 4);
    close(writeEnd);

    read(readEnd, buf, 4);
    close(readEnd);

    printf("%d: received %s\n", myPid, buf);
  }
  exit(0);
}
