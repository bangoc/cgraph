#ifndef GETLINE_H_
#define GETLINE_H_

#include <stdio.h>

/*
 * Nhập dòng bỏ qua ký tự xuống dòng
 */

long my_getline(char **lineptr, long *n, FILE *stream);

#endif   // GETLINE_H_