#include "s21_common.h"

#include <errno.h>
#include <limits.h>
#include <math.h>

#include "s21_string.h"
#include "stdlib.h"

// #define S21_MAX_EXP 4932

int s21_isdigit(const char ch) { return (ch > 47 && ch < 58) ? 1 : 0; }

int s21_isspace(int ch) {
  char *temp = " \r\t\v\f\n";
  return (s21_strchr(temp, ch) == S21_NULL) ? 0 : 1;
}

char *s21_strdup(const char *s) {
  if (s == S21_NULL) return S21_NULL;
  s21_size_t len = s21_strlen(s) + 1;
  char *ptr = malloc(len);
  if (ptr) s21_memcpy(ptr, s, len);
  return ptr;
}

int s21_atoi(const char **str) {
  int num = 0;
  while (s21_isdigit(**str)) {
    num = num * 10 + (**str - '0');
    (*str)++;
  }
  return num;
}