#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>

#include "s21_common.h"
#include "s21_string.h"

typedef struct {
  char **parts;
  char **formats;
  int *widths;
  char **length_mods;
  int parts_count;
  int formats_count;
} format_t;

format_t parse_format(const char *fmt);
void free_format(format_t *f);

int s21_sscanf(const char *str, const char *format, ...) {
    if (!str || !format) return -1;

    int result = 0;
    int conversion_success = 0;
    format_t f = parse_format(format);
    if (f.formats_count == 0 && f.parts_count == 0) {
        return -1; // Ошибка парсинга формата
    }

    va_list list;
    va_start(list, format);
    const char *begin = str;

  for (int i = 0; i < f.formats_count; i++) {
    if (i < f.parts_count && f.parts[i] && f.parts[i][0] != '\0') {
      char *ptr = s21_strstr(begin, f.parts[i]);
      if (!ptr) break;
      begin = ptr + s21_strlen(f.parts[i]);
    }

    if (s21_strncmp(f.formats[i], "c", 1) != 0 && f.formats[i][0] != '[') {
      while (s21_isspace(*begin)) begin++;
    }

    void *void_ptr = va_arg(list, void *);
    if (!void_ptr) continue;

    const char *end_ptr = begin;
    int width = f.widths[i];
    conversion_success = 0;

    if (s21_strncmp(f.formats[i], "s", 1) == 0) {
      char *str_ptr = (char *)void_ptr;
      int count = 0;
      int max_chars = width > 0 ? width : INT_MAX;

      while (*begin && !s21_isspace(*begin) && count < max_chars) {
        *str_ptr++ = *begin++;
        count++;
      }
      *str_ptr = '\0';
      end_ptr = begin;
      if (count > 0) {
        conversion_success = 1;
        result++;
      } else {
        break;
      }
    } else if (s21_strncmp(f.formats[i], "d", 1) == 0 ||
               s21_strncmp(f.formats[i], "i", 1) == 0) {
      long long val = 0;
      if (f.length_mods[i] && s21_strncmp(f.length_mods[i], "l", 1) == 0) {
        long *ptr = (long *)void_ptr;
        val = strtol(begin, (char **)&end_ptr,
                     s21_strncmp(f.formats[i], "i", 1) == 0 ? 0 : 10);
        *ptr = (long)val;
      } else if (f.length_mods[i] &&
                 s21_strncmp(f.length_mods[i], "ll", 2) == 0) {
        long long *ptr = (long long *)void_ptr;
        val = strtoll(begin, (char **)&end_ptr,
                      s21_strncmp(f.formats[i], "i", 1) == 0 ? 0 : 10);
        *ptr = val;
      } else if (f.length_mods[i] &&
                 s21_strncmp(f.length_mods[i], "h", 1) == 0) {
        short *ptr = (short *)void_ptr;
        val = strtol(begin, (char **)&end_ptr,
                     s21_strncmp(f.formats[i], "i", 1) == 0 ? 0 : 10);
        *ptr = (short)val;
      } else if (f.length_mods[i] &&
                 s21_strncmp(f.length_mods[i], "hh", 2) == 0) {
        signed char *ptr = (signed char *)void_ptr;
        val = strtol(begin, (char **)&end_ptr,
                     s21_strncmp(f.formats[i], "i", 1) == 0 ? 0 : 10);
        *ptr = (signed char)val;
      } else {
        int *ptr = (int *)void_ptr;
        val = strtol(begin, (char **)&end_ptr,
                     s21_strncmp(f.formats[i], "i", 1) == 0 ? 0 : 10);
        *ptr = (int)val;
      }
      if (end_ptr != begin) {
        conversion_success = 1;
        result++;
      } else {
        break;
      }
    } else if (s21_strncmp(f.formats[i], "f", 1) == 0 ||
               s21_strncmp(f.formats[i], "e", 1) == 0 ||
               s21_strncmp(f.formats[i], "E", 1) == 0 ||
               s21_strncmp(f.formats[i], "g", 1) == 0 ||
               s21_strncmp(f.formats[i], "G", 1) == 0) {
      // Обработка чисел с плавающей точкой
      if (f.length_mods[i] && f.length_mods[i][0] == 'L') {
        long double *ptr = (long double *)void_ptr;
        *ptr = strtold(begin, (char **)&end_ptr);
      } else {
        float *ptr = (float *)void_ptr;
        *ptr = strtof(begin, (char **)&end_ptr);
      }
      if (end_ptr != begin) {
        conversion_success = 1;
        result++;
      } else {
        break;
      }
    } else if (s21_strncmp(f.formats[i], "c", 1) == 0) {
      char *ptr = (char *)void_ptr;
      int count = width > 0 ? width : 1;
      while (count-- > 0 && *begin) {
        *ptr++ = *begin++;
      }
      end_ptr = begin;
      conversion_success = 1;
      result++;
    } else if (f.formats[i][0] == '[') {
    char *ptr = (char *)void_ptr;
    int invert = 0;
    const char *set = f.formats[i] + 1; // Пропускаем '['
    
    // Проверяем на отрицательное множество (начинается с '^')
    if (*set == '^') {
        invert = 1;
        set++;
    }
    
    int count = 0;
    int max_chars = width > 0 ? width : INT_MAX;
    
    while (*begin && count < max_chars) {
        int match = 0;
        const char *temp = set;
        
        // Проверяем, есть ли символ в множестве
        while (*temp != ']' && *temp != '\0') {
            if (*temp == '-' && *(temp + 1) != ']' && temp > set) {
                // Обработка диапазона (a-z, 0-9 и т.д.)
                if (*begin >= *(temp - 1) && *begin <= *(temp + 1)) {
                    match = 1;
                    break;
                }
                temp++;
            } else if (*begin == *temp) {
                match = 1;
                break;
            }
            temp++;
        }
        
        if ((match && !invert) || (!match && invert)) {
            *ptr++ = *begin++;
            count++;
        } else {
            break;
        }
    }
    
    *ptr = '\0';
    end_ptr = begin;
    if (count > 0) {
        conversion_success = 1;
        result++;
    } else {
        break;
    }
}

    if (conversion_success) {
      begin = end_ptr;
    } else {
      break;
    }
  }

  va_end(list);
  free_format(&f);
  return result;
}

format_t parse_format(const char *fmt) {
    format_t f = {0};
    if (!fmt) return f;

    int max_parts = s21_strlen(fmt) + 1;
    f.parts = calloc(max_parts, sizeof(char *));
    f.formats = calloc(max_parts, sizeof(char *));
    f.widths = calloc(max_parts, sizeof(int));
    f.length_mods = calloc(max_parts, sizeof(char *));

    if (!f.parts || !f.formats || !f.widths || !f.length_mods) {
        free(f.parts);
        free(f.formats);
        free(f.widths);
        free(f.length_mods);
        s21_memset(&f, 0, sizeof(f));
        return f;
    }

    char *fmt_copy = s21_strdup(fmt);
    if (!fmt_copy) {
        free_format(&f);
        s21_memset(&f, 0, sizeof(f));
        return f;
    }

    char *begin = fmt_copy;
    char *percent = S21_NULL;
    int i = 0, j = 0;

    while ((percent = s21_strchr(begin, '%')) != S21_NULL) {
        *percent = '\0';
        f.parts[i] = s21_strdup(begin);
        *percent = '%';
        if (!f.parts[i]) {
            free(fmt_copy);
            free_format(&f);
            s21_memset(&f, 0, sizeof(f));
            return f;
        }
        i++;

        if (percent[1] == '%') {
            begin = percent + 2;
            continue;
        }

        char *spec_start = percent + 1;
        char *spec_end = spec_start;

        int width = 0;
        while (s21_isdigit(*spec_end)) {
            width = width * 10 + (*spec_end - '0');
            spec_end++;
        }
        f.widths[j] = width;

        if (s21_strncmp(spec_end, "hh", 2) == 0) {
            f.length_mods[j] = s21_strdup("hh");
            spec_end += 2;
        } else if (s21_strncmp(spec_end, "ll", 2) == 0) {
            f.length_mods[j] = s21_strdup("ll");
            spec_end += 2;
        } else if (*spec_end == 'h' || *spec_end == 'l' || *spec_end == 'L') {
            f.length_mods[j] = malloc(2);
            if (f.length_mods[j]) {
                f.length_mods[j][0] = *spec_end;
                f.length_mods[j][1] = '\0';
            }
            spec_end++;
        }

        while (*spec_end && !s21_strchr("diuoxXfFeEgGcsn%[]", *spec_end)) {
            spec_end++;
        }

        if (*spec_end) {
            if (*spec_end == '[') {
                char *close_bracket = s21_strchr(spec_end, ']');
                if (close_bracket) {
                    int len = close_bracket - spec_end + 1;
                    f.formats[j] = malloc(len + 1);
                    if (f.formats[j]) {
                        s21_strncpy(f.formats[j], spec_end, len);
                        f.formats[j][len] = '\0';
                    }
                    spec_end = close_bracket + 1;
                } else {
                    free(fmt_copy);
                    free_format(&f);
                    s21_memset(&f, 0, sizeof(f));
                    return f;
                }
            } else {
                f.formats[j] = malloc(2);
                if (f.formats[j]) {
                    f.formats[j][0] = *spec_end;
                    f.formats[j][1] = '\0';
                }
                spec_end++;
            }
            j++;
        }
        begin = spec_end;
    }

    if (*begin != '\0') {
        f.parts[i] = s21_strdup(begin);
        if (!f.parts[i]) {
            free(fmt_copy);
            free_format(&f);
            s21_memset(&f, 0, sizeof(f));
            return f;
        }
        i++;
    }

    free(fmt_copy);
    f.parts_count = i;
    f.formats_count = j;
    return f;
}

void free_format(format_t *f) {
  if (!f) return;

  for (int i = 0; i < f->parts_count; i++) {
    if (f->parts[i]) free(f->parts[i]);
  }
  for (int i = 0; i < f->formats_count; i++) {
    if (f->formats[i]) free(f->formats[i]);
    if (f->length_mods[i]) free(f->length_mods[i]);
  }
  if (f->parts) free(f->parts);
  if (f->formats) free(f->formats);
  if (f->widths) free(f->widths);
  if (f->length_mods) free(f->length_mods);
}