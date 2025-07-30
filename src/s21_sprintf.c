#include <math.h>
#include <stdarg.h>
#include <stdint.h>

#include "s21_common.h"
#include "s21_string.h"

typedef struct {
  int minus;
  int plus;
  int space;
  int zero;
  int hash;
  int width;
  int precision;
  int is_precision_set;
  char length;
  char specifier;
} s21_format;

static void parse_flags(const char **format, s21_format *fmt);
static void parse_width(const char **format, s21_format *fmt, va_list args);
static void parse_precision(const char **format, s21_format *fmt, va_list args);
static void parse_length(const char **format, s21_format *fmt);
static void parse_specifier(const char **format, s21_format *fmt);
static void parse_format(const char **format, s21_format *fmt, va_list args);
static void process_char(char **str, s21_format *fmt, va_list args);
static void process_string(char **str, s21_format *fmt, va_list args);
static void process_int(char **str, s21_format *fmt, va_list args);
static void process_unsigned(char **str, s21_format *fmt,
                             long long unsigned number);
static void process_g_format(char **str, s21_format *fmt, double num);
static void process_exponential(char **str, s21_format *fmt, double num);
static void process_float(char **str, s21_format *fmt, double num);
static void process_percent(char **str, s21_format *fmt);
int s21_sprintf(char *str, const char *format, ...);

static void parse_flags(const char **format, s21_format *fmt) {
  while (1) {
    switch (**format) {
      case '-':
        fmt->minus = 1;
        break;
      case '+':
        fmt->plus = 1;
        break;
      case ' ':
        fmt->space = 1;
        break;
      case '#':
        fmt->hash = 1;
        break;
      case '0':
        fmt->zero = 1;
        break;
      default:
        return;
    }
    (*format)++;
  }
}

static void parse_width(const char **format, s21_format *fmt,
                        va_list args) {  // ширина поля
  if (**format == '*') {
    fmt->width = va_arg(args, int);
    if (fmt->width < 0) {
      fmt->minus = 1;
      fmt->width = -fmt->width;
    }
    (*format)++;
  } else {
    fmt->width = s21_atoi(format);
  }
}

static void parse_precision(const char **format, s21_format *fmt,
                            va_list args) {  // точность
  if (**format == '.') {
    (*format)++;
    fmt->is_precision_set = 1;
    if (**format == '*') {
      fmt->precision = va_arg(args, int);
      if (fmt->precision < 0) {
        fmt->is_precision_set = 0;
      }
      (*format)++;
    } else {
      fmt->precision = s21_atoi(format);
    }
  }
}

static void parse_length(const char **format, s21_format *fmt) {  // длинна
  char ll_format = '>';
  char hh_format = '<';

  if (**format == 'l' && *(*format + 1) == 'l') {
    fmt->length = ll_format;
    (*format)++;
    (*format)++;
  } else if (**format == 'h' && *(*format + 1) == 'h') {
    fmt->length = hh_format;
    (*format)++;
    (*format)++;
  } else {
    switch (**format) {
      case 'h':
      case 'l':
      case 'L':
        fmt->length = **format;
        (*format)++;
        break;
      default:
        break;
    }
  }
}

static void parse_specifier(const char **format,
                            s21_format *fmt) {  // спецификаторы формата
  switch (**format) {
    case 'c':
    case 'd':
    case 'i':
    case 'e':
    case 'E':
    case 'f':
    case 'g':
    case 'G':
    case 'o':
    case 's':
    case 'u':
    case 'x':
    case 'X':
    case 'p':
    case 'n':
    case '%':
      fmt->specifier = **format;
      (*format)++;
      break;
    default:
      break;
  }
}

static void parse_format(const char **format, s21_format *fmt, va_list args) {
  parse_flags(format, fmt);
  parse_width(format, fmt, args);
  parse_precision(format, fmt, args);
  parse_length(format, fmt);
  parse_specifier(format, fmt);
}

static void process_char(
    char **str, s21_format *fmt,
    va_list args) {  // Форматирую символ с учетом ширины и выравнивания
  char c = (char)va_arg(args, int);
  if (fmt->minus) {
    *(*str)++ = c;
    while (fmt->width-- > 1) {
      *(*str)++ = ' ';
    }
  } else {
    while (fmt->width-- > 1) {
      *(*str)++ = ' ';
    }
    *(*str)++ = c;
  }
}

static void process_string(char **str, s21_format *fmt,
                           va_list args) {  // Форматирую строку с учетом
                                            // ширины, точности и выравнивания

  char *s = va_arg(args, char *);
  if (s == S21_NULL) {
    s = "(null)";
  }
  s21_size_t len = s21_strlen(s);
  if (fmt->is_precision_set && (s21_size_t)fmt->precision < len) {
    len = fmt->precision;
  }

  if (fmt->minus) {
    for (s21_size_t i = 0; i < len; i++) {
      *(*str)++ = s[i];
    }
    while ((int)len < fmt->width--) {
      *(*str)++ = ' ';
    }
  } else {
    while ((int)len < fmt->width--) {
      *(*str)++ = ' ';
    }
    for (s21_size_t i = 0; i < len; i++) {
      *(*str)++ = s[i];
    }
  }
}

static void process_int(char **str, s21_format *fmt,
                        va_list args) {  // Форматирую целое число
  long num;
  char ll_format = '>';
  char hh_format = '<';
  if (fmt->length == 'l') {
    num = va_arg(args, long);
  } else if (fmt->length == ll_format) {
    num = va_arg(args, long long int);
  } else if (fmt->length == hh_format) {
    num = va_arg(args, int);
  } else {
    num = va_arg(args, int);
  }

  int is_negative = num < 0;
  if (is_negative) {
    num = -num;
  }

  char buffer[32];  // Преобразую числа в строку
  int i = 0;
  do {
    buffer[i++] = '0' + (num % 10);
    num /= 10;
  } while (num > 0);

  int len = i;  // Добавляю ведущих нулей по точности
  int zeros = 0;
  if (fmt->is_precision_set) {
    zeros = fmt->precision - len;
    if (zeros < 0) {
      zeros = 0;
    }
    len += zeros;
  }

  int sign_len = 0;
  if (is_negative) {
    sign_len = 1;
  } else if (fmt->plus) {
    sign_len = 1;
  } else if (fmt->space) {
    sign_len = 1;
  }

  int total_len = len + sign_len;
  int padding = fmt->width - total_len;
  if (padding < 0) {
    padding = 0;
  }

  if (!fmt->minus && !fmt->zero) {
    while (padding-- > 0) {
      *(*str)++ = ' ';
    }
  }

  if (is_negative) {
    *(*str)++ = '-';
  } else if (fmt->plus) {
    *(*str)++ = '+';
  } else if (fmt->space) {
    *(*str)++ = ' ';
  }

  if (!fmt->minus && fmt->zero) {
    while (padding-- > 0) {
      *(*str)++ = '0';
    }
  }

  while (zeros-- > 0) {
    *(*str)++ = '0';
  }

  while (i-- > 0) {
    *(*str)++ = buffer[i];
  }

  if (fmt->minus) {
    while (padding-- > 0) {
      *(*str)++ = ' ';
    }
  }
}
// добавили long long для корректного преобразования адреса памяти
static void process_unsigned(char **str, s21_format *fmt,
                             unsigned long long number) {
  unsigned long long num;
  char ll_format = '>';
  if (fmt->length == ll_format) {
    num = number;
  } else if (fmt->length == 'l') {
    num = (unsigned long)number;
  } else {
    num = (unsigned int)number;
  }

  char buffer[32];
  int base = 10;
  char *digits = "0123456789abcdef";

  if (fmt->specifier == 'o') {
    base = 8;
  } else if (fmt->specifier == 'x' || fmt->specifier == 'X') {
    base = 16;
    if (fmt->specifier == 'X') {
      digits = "0123456789ABCDEF";
    }
  }

  int i = 0;
  do {
    buffer[i++] = digits[num % base];
    num /= base;
  } while (num > 0);

  int len = i;
  int zeros = 0;
  if (fmt->is_precision_set) {
    zeros = fmt->precision - len;
    if (zeros < 0) {
      zeros = 0;
    }
    len += zeros;
  }

  int prefix_len = 0;
  if (fmt->hash) {
    if ((zeros == 0 || buffer[i - 1] != '0')) {
      if (fmt->specifier == 'o') {
        prefix_len = 1;
      } else if ((fmt->specifier == 'x' || fmt->specifier == 'X')) {
        prefix_len = 2;
      }
    }
  }

  int total_len = len + prefix_len;
  int padding = fmt->width - total_len;
  if (padding < 0) {
    padding = 0;
  }

  if (!fmt->minus && !fmt->zero) {
    while (padding-- > 0) {
      *(*str)++ = ' ';
    }
  }

  if (prefix_len > 0) {
    if (fmt->specifier == 'o') {
      *(*str)++ = '0';
    } else if (fmt->specifier == 'x') {
      *(*str)++ = '0';
      *(*str)++ = fmt->specifier;
    } else if (fmt->specifier == 'X') {
      *(*str)++ = '0';
      *(*str)++ = fmt->specifier;
    }
  }

  if (!fmt->minus && fmt->zero) {
    while (padding-- > 0) {
      *(*str)++ = '0';
    }
  }

  while (zeros-- > 0) {
    *(*str)++ = '0';
  }

  while (i-- > 0) {
    *(*str)++ = buffer[i];
  }

  if (fmt->minus) {
    while (padding-- > 0) {
      *(*str)++ = ' ';
    }
  }
}

static void process_float(char **str, s21_format *fmt, double num) {
  int is_negative = num < 0;
  if (is_negative) num = -num;

  if (!fmt->is_precision_set) fmt->precision = 6;

  // Округляем число до нужной точности
  double power = pow(10, fmt->precision);
  double rounded = round(num * power) / power;

  // Разделяем на целую и дробную части
  long int_part = (long)rounded;
  double frac = rounded - int_part;
  long frac_part = (long)(frac * power + 0.5);

  // Проверка переполнения при округлении
  if (frac_part >= (long)power) {
    frac_part = 0;
    int_part++;
  }

  // Формируем строку
  char buffer[512];
  int pos = 0;

  // Знак
  if (is_negative)
    buffer[pos++] = '-';
  else if (fmt->plus)
    buffer[pos++] = '+';
  else if (fmt->space)
    buffer[pos++] = ' ';

  // Целая часть
  long temp = int_part;
  do {
    buffer[pos++] = '0' + (temp % 10);
    temp /= 10;
  } while (temp > 0);

  // Разворачиваем целую часть
  int int_len = pos - (is_negative || fmt->plus || fmt->space);
  for (int i = 0; i < int_len / 2; i++) {
    char tmp = buffer[pos - int_len + i];
    buffer[pos - int_len + i] = buffer[pos - 1 - i];
    buffer[pos - 1 - i] = tmp;
  }

  // Дробная часть
  if (fmt->precision > 0 || fmt->hash) {
    buffer[pos++] = '.';
    // Записываем цифры в правильном порядке
    for (int i = 0; i < fmt->precision; i++) {
      buffer[pos + fmt->precision - 1 - i] = '0' + (frac_part % 10);
      frac_part /= 10;
    }
    pos += fmt->precision;
  }

  // Добавляем в выходную строку с учетом ширины
  int len = pos;
  int padding = fmt->width - len;

  if (!fmt->minus && !fmt->zero) {
    while (padding-- > 0) *(*str)++ = ' ';
  }

  s21_memcpy(*str, buffer, pos);
  *str += pos;

  if (fmt->minus) {
    while (padding-- > 0) *(*str)++ = ' ';
  }
}

static void process_g_format(char **str, s21_format *fmt, double num) {
  int is_negative = num < 0;
  if (is_negative) num = -num;

  if (!fmt->is_precision_set) fmt->precision = 6;
  if (fmt->precision == 0) fmt->precision = 1;

  // Определяем экспоненту
  int exponent = (num == 0) ? 0 : (int)floor(log10(fabs(num)));

  // Решаем, использовать ли экспоненциальную запись
  int use_exp = (exponent < -4 || exponent >= fmt->precision);

  if (use_exp) {
    // Экспоненциальная форма (e/E)
    fmt->precision--;
    process_exponential(str, fmt, is_negative ? -num : num);
  } else {
    // Обычная форма (f)
    int new_precision = fmt->precision - (exponent + 1);
    if (new_precision < 0) new_precision = 0;

    // Формируем временную строку
    char temp_buf[512];
    char *temp_str = temp_buf;

    // Форматируем с нужной точностью (без учета ширины)
    s21_format temp_fmt = *fmt;
    temp_fmt.precision = new_precision;
    temp_fmt.is_precision_set = 1;
    temp_fmt.specifier = 'f';
    temp_fmt.width = 0;  // Временное форматирование без ширины
    process_float(&temp_str, &temp_fmt, is_negative ? -num : num);

    // Удаляем незначащие нули
    char *dot = s21_strchr(temp_buf, '.');
    if (dot != S21_NULL) {
      char *end = temp_str - 1;
      while (end > dot && *end == '0') {
        *end-- = '\0';
      }
      if (end == dot && !fmt->hash) {
        *dot = '\0';
      }
    }

    // Теперь обрабатываем ширину
    int len = s21_strlen(temp_buf);
    int padding = fmt->width - len;

    if (!fmt->minus && padding > 0) {
      // Добавляем пробелы слева
      while (padding-- > 0) {
        *(*str)++ = ' ';
      }
    }

    // Копируем основную строку
    s21_memcpy(*str, temp_buf, len);
    *str += len;

    if (fmt->minus && padding > 0) {
      // Добавляем пробелы справа
      while (padding-- > 0) {
        *(*str)++ = ' ';
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void process_exponential(char **str, s21_format *fmt, double num) {
  int is_negative = num < 0;
  if (is_negative) num = -num;

  if (!fmt->is_precision_set) fmt->precision = 6;

  // Определяем экспоненту
  int exponent = 0;
  if (num != 0) {
    exponent = (int)floor(log10(num));
    num /= pow(10, exponent);
  }

  // Нормализуем число: 1.0 <= num < 10.0
  if (num >= 10.0) {
    num /= 10.0;
    exponent++;
  } else if (num < 1.0 && num != 0) {
    num *= 10.0;
    exponent--;
  }

  // Округляем с учётом precision (более точный способ)
  double rounder = pow(10, fmt->precision);
  num = round(num * rounder) / rounder;

  // Проверка переполнения после округления
  if (num >= 10.0) {
    num /= 10.0;
    exponent++;
  }

  // Целая часть (1 цифра)
  int int_part = (int)num;
  num -= int_part;

  // Дробная часть
  long frac_part = 0;
  if (fmt->precision > 0) {
    frac_part = (long)(num * pow(10, fmt->precision) +
                       0.5);  // Дополнительное округление для точности
  }

  // Обработка переполнения (если frac_part == 10^precision)
  if (frac_part >= (long)pow(10, fmt->precision)) {
    frac_part = 0;
    int_part++;
    if (int_part >= 10) {
      int_part = 1;
      exponent++;
    }
  }

  // Формируем строку
  char buffer[512];
  int pos = 0;

  // Знак
  if (is_negative)
    buffer[pos++] = '-';
  else if (fmt->plus)
    buffer[pos++] = '+';
  else if (fmt->space)
    buffer[pos++] = ' ';

  // Целая часть
  buffer[pos++] = '0' + int_part;

  // Дробная часть (если precision > 0 или есть флаг #)
  if (fmt->precision > 0 || fmt->hash) {
    buffer[pos++] = '.';
    for (int i = fmt->precision - 1; i >= 0; i--) {
      buffer[pos + i] = '0' + (frac_part % 10);
      frac_part /= 10;
    }
    pos += fmt->precision;
  }

  // Экспонента
  buffer[pos++] = (fmt->specifier == 'E' || fmt->specifier == 'G') ? 'E' : 'e';
  buffer[pos++] = (exponent < 0) ? '-' : '+';
  exponent = (exponent < 0) ? exponent * (-1) : exponent;
  buffer[pos++] = '0' + (exponent / 10);
  buffer[pos++] = '0' + (exponent % 10);
  buffer[pos] = '\0';

  // Добавляем в выходную строку с учётом ширины
  int len = pos;
  int padding = fmt->width - len;

  if (!fmt->minus && !fmt->zero) {
    while (padding-- > 0) *(*str)++ = ' ';
  }

  if (!fmt->minus && fmt->zero) {
    if (buffer[0] == '-' || buffer[0] == '+' || buffer[0] == ' ') {
      *(*str)++ = buffer[0];
      s21_memcpy(*str, buffer + 1, len - 1);
      *str += len - 1;
    } else {
      while (padding-- > 0) *(*str)++ = '0';
      s21_memcpy(*str, buffer, len);
      *str += len;
    }
  } else {
    s21_memcpy(*str, buffer, len);
    *str += len;
  }

  if (fmt->minus) {
    while (padding-- > 0) *(*str)++ = ' ';
  }
}

static void process_percent(char **str, s21_format *fmt) {
  if (fmt->minus) {
    *(*str)++ = '%';
    while (fmt->width-- > 1) {
      *(*str)++ = ' ';
    }
  } else {
    while (fmt->width-- > 1) {
      *(*str)++ = fmt->zero ? '0' : ' ';
    }
    *(*str)++ = '%';
  }
}

int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *start = str;

  while (*format) {
    if (*format != '%') {
      *str++ = *format++;
    } else {
      format++;
      s21_format fmt = {0};
      parse_format(&format, &fmt, args);
      switch (fmt.specifier) {
        case 'c':
          process_char(&str, &fmt, args);
          break;
        case 'd':
        case 'i':
          process_int(&str, &fmt, args);
          break;
        case 's':
          process_string(&str, &fmt, args);
          break;
        case 'u':
        case 'o':
        case 'x':
        case 'X':
          void *ptr = va_arg(args, void *);
          unsigned long long num = (unsigned long long)(uintptr_t)ptr;
          process_unsigned(&str, &fmt, num);
          break;
        case 'f': {
          double num = (fmt.length == 'L') ? va_arg(args, long double)
                                           : va_arg(args, double);
          process_float(&str, &fmt, num);
          break;
        }
        case 'e':
        case 'E': {
          double num = (fmt.length == 'L') ? va_arg(args, long double)
                                           : va_arg(args, double);
          process_exponential(&str, &fmt, num);
          break;
        }
        case 'g':
        case 'G': {
          double num = (fmt.length == 'L') ? va_arg(args, long double)
                                           : va_arg(args, double);
          process_g_format(&str, &fmt, num);
          break;
        }
        case 'p': {
          void *ptr = va_arg(args, void *);
          fmt.specifier = 'x';
          fmt.hash = 1;
          fmt.length = '>';
          unsigned long long num = (unsigned long long)(uintptr_t)ptr;
          process_unsigned(&str, &fmt, num);
          break;
        }
        case '%':
          process_percent(&str, &fmt);
          break;
        default:
          break;
      }
    }
  }
  *str = '\0';
  va_end(args);
  return (int)(str - start);
}
