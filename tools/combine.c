/*
  @bangoc 2021
  The purpose of this tool is to combine all files into a large
  translation unit
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char *includes[1024];
char *others[1024 * 1024];
int inc_idx = 0;
int oth_idx = 0;

char *make_copy(const char *buff) {
  int len = strlen(buff);
  char *s = malloc(len + 1);
  strcpy(s, buff);
  return s;
}

bool is_include(const char *line) {
  return strstr(line, "#include") == line;
}

bool is_standard_include(const char *line) {
  return strstr(line, "#include <") == line;
}

void ensure_newline(char *line) {
  int len = strlen(line);
  if (line[len - 1] != '\n') {
    line[len] = '\n';
    line[len + 1] = '\0';
  }
}

void parse_file(const char *fname) {
  char buff[1024];
  FILE *inp = fopen(fname, "r");
  while (fgets(buff, 1024, inp) ) {
    ensure_newline(buff);
    char *line = make_copy(buff);
    if (is_include(line)) {
      if (is_standard_include(line)) {
        bool existed = false;
        for (int i = 0; i < inc_idx; ++i) {
          if (strcmp(includes[i], line) == 0) {
            existed = true;
            break;
          }
        }
        if (existed) {
          free(line);
        } else {
          includes[inc_idx++] = line;
        }
      } else {
        free(line);
      }
    } else {
      others[oth_idx++] = line;
    }
  }
  fclose(inp);
}

int my_strcmp(const void *s1, const void *s2) {
  return strcmp(*(const char**)s1, *(const char**)s2);
}

void remove_newline(char *s) {
  int len = strlen(s);
  while (s[len - 1] == '\n' || s[len - 1] == '\r') {
    s[len - 1] = '\0';
    --len;
  }
}

int main(int argc, char *argv[]) {
  // Usage: ./cb prefix list.txt
  //  Where prefix is the prefix of all sources file
  char out_name[1024],
       buffer[1024], fname[1024];
  FILE *list = fopen(argv[2], "r");
  fgets(out_name, 1024, list);
  remove_newline(out_name);
  while (fgets(buffer, 1024, list)) {
    remove_newline(buffer);
    strcpy(fname, argv[1]);
    strcat(fname, buffer);
    parse_file(fname);
  }
  fclose(list);
  FILE *o = fopen(out_name, "w");
  qsort(includes, inc_idx, sizeof(includes[0]), my_strcmp);
  for (int i = 0; i < inc_idx; ++i) {
    fprintf(o, "%s", includes[i]);
  }
  for (int i = 0; i < oth_idx; ++i) {
    fprintf(o, "%s", others[i]);
  }
  fclose(o);
  return 0;
}