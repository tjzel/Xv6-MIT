#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  uint firstPipe[2];
  uint secondPipe[2];
  pipe(firstPipe);
  pipe(secondPipe);
  const uint pid = fork();
  const uint myPid = getpid();
  if (pid == 0)
  {
    const uint readEnd = firstPipe[0];
    const uint writeEnd = secondPipe[1];
    close(firstPipe[1]);
    close(secondPipe[0]);
    char buf[5];
    read(5, buf, readEnd);
    close(readEnd);
    printf("%d received %s", &myPid, buf);
    *buf = "pong";
    write(5, buf, writeEnd);
    close(writeEnd);
  }
  else
  {
    const uint readEnd = secondPipe[0];
    const uint writeEnd = firstPipe[1];
    close(firstPipe[0]);
    close(secondPipe[1]);
    char buf[5] = "ping";
    buf[4] = 0;
    write(5, buf, writeEnd);
    close(writeEnd);
    wait(0);
    read(5, buf, readEnd);
    close(readEnd);
    printf("%d received %s", &myPid, buf);
  }
  exit(0);
}
