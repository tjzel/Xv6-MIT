#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieveChild(const int readEnd) {
  if (fork() == 0) {
    int prime;
    if (read(readEnd, &prime, 4) != 4) exit(0);
    printf("prime %d\n", prime);

    int pipeline[2];
    pipe(pipeline);
    const int childReadEnd = pipeline[0];
    const int writeEnd = pipeline[1];

    int value = -1;
    while (read(readEnd, &value, 4) == 4)
      if (value % prime) write(writeEnd, &value, 4);
    close(readEnd);
    close(writeEnd);

    sieveChild(childReadEnd);
    close(childReadEnd);

    wait(0);
    exit(0);
  }
}

int main(int argc, char* argv[]) {
  int pipeline[2];
  pipe(pipeline);

  const int readEnd = pipeline[0];
  const int writeEnd = pipeline[1];

  const int start = 2;
  const int end = 35;
  for (int i = start; i <= end; ++i) {
    write(writeEnd, &i, 4);
  }
  close(writeEnd);

  sieveChild(readEnd);
  close(readEnd);

  while (wait(0) != -1)
    ;

  exit(0);
}
