#include "s21_string.h"

#include <stdlib.h>

#include "s21_error_desc.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *result = S21_NULL;
  const char *base = (char *)str;
  const char *p = (char *)str;
  int size = (int)n;
  while (p - base < size) {
    if (*p == c) {
      result = (void *)p;
      break;
    }
    p++;
  }
  return result;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int res = 0;
  char *base = (char *)str1;
  char *p1 = (char *)str1;
  char *p2 = (char *)str2;
  int size = (int)n;
  while (p1 - base < size) {
    if (*p1 != *p2) {
      res = *p1 - *p2;
      break;
    }
    p1++;
    p2++;
  }
  return res;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *p = dest;
  const char *q = src;
  unsigned long int count = 0;
  while (count < n) {
    *p++ = *q++;
    count++;
  }
  return (void *)p;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  char value = (char)c;
  char *p = (char *)str;
  unsigned long int count = 0;
  while (count < n) {
    *p++ = value;
    count++;
  }
  return str;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t dest_len = s21_strlen(dest);
  s21_size_t src_len = s21_strlen(src);
  for (unsigned int i = 0; i < src_len; i++) {
    if (i >= n) {
      src_len = i;
      break;
    }
    dest[dest_len + i] = src[i];
  }
  dest[dest_len + src_len] = '\0';
  return dest;
}

char *s21_strchr(const char *str, int c) {
    if (str == S21_NULL) return S21_NULL;
    
    for (; *str != '\0'; str++) {
        if (*str == c) {
            return (char *)str;
        }
    }
    
    if (c == '\0') {
        return (char *)str;
    }
    
    return S21_NULL;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  const char *p = str1;
  const char *q = str2;
  int res = 0;
  for (unsigned long int i = 0; *p != '\0' && *q != '\0' && i < n;
       p++, q++, i++) {
    if (*p != *q) {
      res = *p - *q;
    }
  }
  return res;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
    if (dest == S21_NULL || src == S21_NULL) {
        return S21_NULL;
    }

    char *start = dest;
    s21_size_t i = 0;

    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    while (i < n) {
        dest[i] = '\0';
        i++;
    }

    return start;
}

s21_size_t s21_strlen(const char *str) {
  if(str==S21_NULL){return 0;}
  const char *p = str;
  while (*p != '\0') {
    p++;
  }
  s21_size_t result = p - str;
  return result;
}

char *s21_strrchr(const char *str, int c) {
  char *res = S21_NULL;
  char *p = S21_NULL;
  s21_size_t length = s21_strlen(str);
  for (p = (char *)(str + length); p != str; p--) {
    if (*p == c) {
      res = p;
      break;
    }
  }
  if (p == S21_NULL) {
  }
  if (*p == c) {
    res = p;
  }
  return res;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t res = s21_strlen(str1);
  s21_size_t length = s21_strlen(str2);
  for (unsigned long int i = 0; i <= length; i++) {
    char c = str2[i];
    char *p = s21_strchr(str1, c);
    s21_size_t temp = (p != S21_NULL) ? (unsigned long int)(p - str1) : res;
    if (temp < res) {
      res = temp;
    }
  }
  return res;
}

char *s21_strerror(int errnum) {
  char *p = S21_NULL;
  if (errnum > 133) {
    p = errors[134];
  } else {
    p = errors[errnum];
  }
  return p;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  s21_size_t length = s21_strlen(str2);
  char *res = S21_NULL;
  for (unsigned long int i = 0; i < length; i++) {
    char c = str2[i];
    char *p = s21_strchr(str1, c);
    if (p != S21_NULL && res == S21_NULL) {
      res = p;
    }
    if (p != S21_NULL && res != S21_NULL) {
      if ((p - res) < 0) {
        res = p;
      }
    }
  }
  return res;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *res = S21_NULL;
  for (char *p = (char *)haystack; *p != '\0'; p++) {
    int flag = 1;
    char *src = p;
    char *dst = (char *)needle;
    while (*src != '\0' && *dst != '\0') {
      if (*src != *dst) {
        flag = 0;
        break;
      }
      src++;
      dst++;
    }
    if (flag == 0) {
      continue;
    }
    if (flag == 1) {
      res = p;
      break;
    }
  }
  return res;
}

char *s21_strtok(char *str, const char *delim) {
  static char *text = S21_NULL;
  s21_size_t length = 0;
  char *res = S21_NULL;

  if (str != S21_NULL) {
    text = str;

    while (1) {
      char *p = s21_strpbrk(text, delim);
      if (p != S21_NULL && (p - text) == 0) {
        text = text + 1;
        continue;
      } else if (p != S21_NULL) {
        length = p - text;
        res = text;
        res[length] = '\0';
        text = text + length + 1;
        break;
      } else
        break;
    }
  }

  if (str == S21_NULL) {
    while (1) {
      char *p = s21_strpbrk(text, delim);
      if (p != S21_NULL && (p - text) == 0) {
        text = text + 1;
        continue;
      } else if (p != S21_NULL) {
        length = p - text;
        res = text;
        res[length] = '\0';
        text = text + length + 1;
        break;
      } else if (p == S21_NULL && s21_strlen(text) > 0) {
        length = s21_strlen(text);
        res = text;
        text = text + length;
        break;
      } else
        break;
    }
  }

  return res;
}

void *s21_to_upper(const char *str) {
  if (str == S21_NULL) {
    return S21_NULL;
  }
  s21_size_t len = s21_strlen(str);
  char *temp = malloc(len + 1);
  char *p = temp;
  char *q = (char *)str;
  while (*q != '\0') {
    if (*q >= 97 && *q <= 122) {
      *p = (*q) - 32;
    } else {
      *p = *q;
    }
    q++;
    p++;
  }
  *p = '\0';
  return temp;
}

void *s21_to_lower(const char *str) {
  if (str == S21_NULL) {
    return S21_NULL;
  }
  s21_size_t len = s21_strlen(str);
  char *temp = malloc(len + 1);
  char *p = temp;
  char *q = (char *)str;
  while (*q != '\0') {
    if (*q >= 65 && *q <= 90) {
      *p = (*q) + 32;
    } else {
      *p = *q;
    }
    q++;
    p++;
  }
  *p = '\0';
  return temp;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (src == S21_NULL || str == S21_NULL || start_index > s21_strlen(src)) {
    return S21_NULL;
  }
  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);
  s21_size_t total_len = src_len + str_len;
  char *temp = malloc(total_len + 1);
  temp[total_len] = '\0';
  s21_memcpy(temp, src, start_index);
  char *p = temp + start_index;
  s21_memcpy(p, str, str_len);
  p = p + str_len;
  s21_memcpy(p, src + start_index, src_len - start_index);
  return temp;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (src == S21_NULL || trim_chars == S21_NULL) {
    return S21_NULL;
  }

  s21_size_t src_len = s21_strlen(src);
  char *temp = malloc(src_len + 1);
  if (temp == S21_NULL) {
    return S21_NULL;  // Ошибка выделения памяти
  }

  // Если trim_chars пуст, просто копируем src
  if (s21_strlen(trim_chars) == 0) {
    s21_memcpy(temp, src, src_len + 1);
    return temp;
  }

  // Находим начало строки без trim_chars
  const char *start = src;
  while (*start != '\0' && s21_strchr(trim_chars, *start) != S21_NULL) {
    start++;
  }

  // Находим конец строки без trim_chars
  const char *end = src + src_len - 1;
  while (end >= start && s21_strchr(trim_chars, *end) != S21_NULL) {
    end--;
  }

  // Копируем результат
  s21_size_t len = (end >= start) ? (end - start + 1) : 0;
  s21_memcpy(temp, start, len);
  temp[len] = '\0';

  return temp;
}