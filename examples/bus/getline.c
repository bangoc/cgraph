#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long my_getline(char **lineptr, long *n, FILE *stream) {
#define BUFF_SIZE 1024
  static char buff[BUFF_SIZE];

  if (lineptr == NULL) {
    return -1;
  }

  if (ferror (stream)) {
    return -1;
  }

  if (feof(stream)) {
    return -1;
  }

  long len = 0;
  if (*n > 0) {
    *lineptr[0] = '\0';
  }

  while (!feof(stream)) {
    fgets(buff, BUFF_SIZE, stream);
    char *np = strchr(buff,'\n');
    if (np) {
      *np = '\0';
    }

    long bufflen = strlen(buff);

    len += bufflen;
    char *ptr = NULL;
    if (len + 1 > *n) {
      ptr = realloc(*lineptr, len + 1);
      if (!ptr) {
        return(-1);
      }
      *lineptr = ptr;
      *n = len + 1;
    }
    strcat(*lineptr, buff);
    if (np) {
      break;
    }
  }
  return len;
}