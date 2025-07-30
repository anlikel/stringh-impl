# Название проекта

Простая реализация библиотеки string.h + функций sprintf/sscanf

## Расположение проекта

- Исходные файлы: `/src`
- Ветка разработки: `develop`

## Реализованные функции

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
char *s21_strrchr(const char *str, int c);
s21_size_t s21_strlen(const char *str);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
int s21_sprintf(char *str, const char *format, ...);
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);
int s21_sscanf(const char *str, const char *format, ...);

### Установка
Компилляция проекта проводится с помощью утилиты make из консоли.

make all - сборка проекта.
make test - модульные тесты всех функций с помощью библиотеки check.h
make gcov_report создание отчета в форме index.html с резултатом тестов.

### Подключение библиотеки
компилляция файла main.c с библиотечными функциями.
gcc -std=c11 -Wall -Wextra -Werror -o main.exe main.c -L. -l:s21_string.a -lm


