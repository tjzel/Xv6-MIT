#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char* path) {
  // static char buf[DIRSIZ + 1];
  char* p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void
find(char* path, const char* filename) {
  char buf[512], * p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    exit(-1);
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    exit(-2);
  }

  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
    fprintf(2, "find: path too long\n");
    exit(-3);
  }
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if (stat(buf, &st) < 0) {
      printf("ls: cannot stat %s\n", buf);
      continue;
    }
    char formattedName[512];
    strcpy(formattedName, fmtname(buf));
    if (st.type == T_DIR && strcmp(formattedName, ".") && strcmp(formattedName, "..")) {
      find(buf, filename);
    } else if (st.type == T_FILE && strcmp(formattedName, filename) == 0) {
      printf("%s\n", buf);
    }

  }
  close(fd);
}

int
main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Filename and path required!\n");
    exit(-1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
