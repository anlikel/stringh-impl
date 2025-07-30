#include <check.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

Suite *string_suite(void);

START_TEST(test_memchr) {
  const char *str = "hello world";
  ck_assert_ptr_eq(memchr(str, 'h', 11), s21_memchr(str, 'h', 11));
  ck_assert_ptr_eq(memchr(str, 'o', 11), s21_memchr(str, 'o', 11));
  ck_assert_ptr_eq(memchr(str, 'd', 11), s21_memchr(str, 'd', 11));
  ck_assert_ptr_eq(s21_memchr("hello", 'o', 3), NULL);
  ck_assert_ptr_null(s21_memchr(str, 'z', 11));
  ck_assert_ptr_null(s21_memchr("", 'a', 0));
}
END_TEST

START_TEST(test_memcmp) {
  ck_assert_int_eq(memcmp("", "", 0), s21_memcmp("", "", 0));
  ck_assert_int_eq(memcmp("hello", "hello", 5),
                   s21_memcmp("hello", "hello", 5));
  ck_assert_int_eq(memcmp("hello", "hellx", 4),
                   s21_memcmp("hello", "hellx", 4));
  ck_assert_int_lt(s21_memcmp("apple", "banana", 5), 0);
  ck_assert_int_gt(s21_memcmp("banana", "apple", 5), 0);
}
END_TEST

START_TEST(test_memcpy) {
  char *dest = malloc(20);
  char *dest_21 = malloc(20);
  const char *src = "test";

  memcpy(dest, src, 5);
  s21_memcpy(dest_21, src, 5);

  ck_assert_int_eq(s21_memcmp(dest, dest_21, 5), 0);

  s21_memset(dest, 0, 20);
  s21_memset(dest_21, 0, 20);

  memcpy(dest, src, 2);
  s21_memcpy(dest_21, src, 2);

  ck_assert_int_eq(s21_memcmp(dest, dest_21, 5), 0);

  free(dest);
  free(dest_21);
}
END_TEST

START_TEST(test_memset) {
  char str[10] = "hello";

  ck_assert_ptr_eq(s21_memset(str, 'x', 3), str);
  ck_assert_str_eq(str, "xxxlo");
  ck_assert_ptr_eq(memset(str, 0, 5), str);
  ck_assert_int_eq(str[0], 0);
}
END_TEST

START_TEST(test_strncat) {
  char dest[20] = "hello";

  ck_assert_ptr_eq(s21_strncat(dest, "", 0), dest);
  ck_assert_str_eq(dest, "hello");

  ck_assert_ptr_eq(s21_strncat(dest, " world", 3), dest);
  ck_assert_str_eq(dest, "hello wo");

  s21_strncat(dest, "rld", 10);
  ck_assert_str_eq(dest, "hello world");
}
END_TEST

START_TEST(test_strchr) {
  const char *str = "hello world";

  ck_assert_ptr_eq(s21_strchr(str, 'h'), str);
  ck_assert_ptr_eq(s21_strchr(str, 'o'), str + 4);
  ck_assert_ptr_eq(s21_strchr(str, 'd'), str + 10);
  ck_assert_ptr_null(s21_strchr(str, 'z'));
  ck_assert_ptr_null(s21_strchr("", 'a'));
  ck_assert_ptr_eq(s21_strchr("hello", '\0'), "hello" + 5);
}
END_TEST

START_TEST(test_strncmp) {
  ck_assert_int_eq(s21_strncmp("", "", 0), 0);
  ck_assert_int_lt(s21_strncmp("apple", "banana", 5), 0);
  ck_assert_int_gt(s21_strncmp("banana", "apple", 5), 0);
  ck_assert_int_eq(s21_strncmp("hello", "hello", 5), 0);
  ck_assert_int_eq(s21_strncmp("hello", "hellx", 4), 0);
  ck_assert_int_lt(s21_strncmp("hello", "hellx", 5), 0);
}
END_TEST

START_TEST(test_strncpy) {
    char * dest1=malloc(20);
    char * dest2=malloc(20);
    
    memset(dest1, 0, 20);
    memset(dest2, 0, 20);
    
    const char *src = "test string";
    
    strncpy(dest1, src, 5);
    s21_strncpy(dest2, src, 5);
    ck_assert_str_eq(dest1, dest2);
    
    memset(dest1, 0, 20);
    memset(dest2, 0, 20);
    strncpy(dest1, src, 20);
    s21_strncpy(dest2, src, 20);
    ck_assert_str_eq(dest1, dest2);
    
    memset(dest1, 'x', 20);
    memset(dest2, 'x', 20);
    strncpy(dest1, src, 1);
    s21_strncpy(dest2, src, 1);
    ck_assert_int_eq(dest1[0], dest2[0]);
    
    ck_assert_ptr_null(s21_strncpy(NULL, src, 5));
    ck_assert_ptr_null(s21_strncpy(dest1, NULL, 5));
    free(dest1);
    free(dest2);
}
END_TEST

START_TEST(test_strrchr) {
  const char *str = "hello world";

  ck_assert_ptr_eq(s21_strrchr(str, 'h'), str);
  ck_assert_ptr_eq(s21_strrchr(str, 'o'), str + 7);
  ck_assert_ptr_eq(s21_strrchr(str, 'd'), str + 10);
  ck_assert_ptr_null(s21_strrchr(str, 'z'));
  ck_assert_ptr_null(s21_strrchr("", 'a'));
  ck_assert_ptr_eq(s21_strrchr("hello", 0), "hello" + 5);
}
END_TEST

START_TEST(test_strlen) {
  ck_assert_int_eq(s21_strlen(""), 0);
  ck_assert_int_eq(s21_strlen("hello"), 5);
  ck_assert_int_eq(s21_strlen("hello world"), 11);
  ck_assert_int_eq(s21_strlen("привет"), 12);
}
END_TEST

START_TEST(test_strcspn) {
  ck_assert_int_eq(s21_strcspn("hello", "abc"), 5);
  ck_assert_int_eq(s21_strcspn("hello", "oe"), 1);
  ck_assert_int_eq(s21_strcspn("hello", "h"), 0);
  ck_assert_int_eq(s21_strcspn("hello", "ol"), 2);
  ck_assert_int_eq(s21_strcspn("", "abc"), 0);
}
END_TEST

START_TEST(test_strpbrk) {
  const char *str = "hello world";

  ck_assert_ptr_eq(s21_strpbrk(str, "h"), str);
  ck_assert_ptr_eq(s21_strpbrk(str, "oe"), str + 1);
  ck_assert_ptr_eq(s21_strpbrk(str, "drl"), str + 2);
  ck_assert_ptr_null(s21_strpbrk(str, "xyz"));
  ck_assert_ptr_null(s21_strpbrk("", "abc"));
}
END_TEST

START_TEST(test_strstr) {
  const char *str = "hello world";

  ck_assert_ptr_eq(s21_strstr(str, "hello"), str);
  ck_assert_ptr_eq(s21_strstr(str, "world"), str + 6);
  ck_assert_ptr_eq(s21_strstr(str, "lo wo"), str + 3);
  ck_assert_ptr_eq(s21_strstr(str, ""), str);
  ck_assert_ptr_null(s21_strstr(str, "xyz"));
  ck_assert_ptr_null(s21_strstr("", "abc"));
}
END_TEST

START_TEST(test_strtok) {
  char str[] = "hello,world,test";
  char *token;

  token = s21_strtok(str, ",");
  ck_assert_ptr_nonnull(token);
  ck_assert_str_eq(token, "hello");

  token = s21_strtok(NULL, ",");
  ck_assert_ptr_nonnull(token);
  ck_assert_str_eq(token, "world");

  token = s21_strtok(NULL, ",");
  ck_assert_ptr_nonnull(token);
  ck_assert_str_eq(token, "test");

  token = s21_strtok(NULL, ",");
  ck_assert_ptr_null(token);

  char str2[] = "one\ttwo three\nfour";
  token = s21_strtok(str2, " \t\n");
  ck_assert_str_eq(token, "one");

  token = s21_strtok(NULL, " \t\n");
  ck_assert_str_eq(token, "two");
}
END_TEST

START_TEST(test_strerror) {
  ck_assert_str_eq(s21_strerror(0), "Success");
  ck_assert_str_eq(s21_strerror(EINVAL), "Invalid argument");
  ck_assert_str_eq(s21_strerror(ENOENT), "No such file or directory");
  ck_assert_ptr_nonnull(s21_strerror(150));
}
END_TEST

START_TEST(test_sprintf_s) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%s", "dfg");
  s21_sprintf(buff2, "abc%s", "dfg");
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-s", "dfg");
  s21_sprintf(buff2, "abc%-s", "dfg");
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_c) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%c", 'g');
  s21_sprintf(buff2, "abc%c", 'g');
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-c", 'g');
  s21_sprintf(buff2, "abc%-c", 'g');
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_d) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%d", 5);
  s21_sprintf(buff2, "abc%d", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-d", 5);
  s21_sprintf(buff2, "abc%-d", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+d", -100);
  s21_sprintf(buff2, "abc%+d", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc% d", -100);
  s21_sprintf(buff2, "abc% d", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%6d", 5);
  s21_sprintf(buff2, "abc%6d", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%06d", 5);
  s21_sprintf(buff2, "abc%06d", 5);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_i) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%i", 5);
  s21_sprintf(buff2, "abc%i", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-i", 5);
  s21_sprintf(buff2, "abc%-i", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+i", -100);
  s21_sprintf(buff2, "abc%+i", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc% i", -100);
  s21_sprintf(buff2, "abc% i", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%6i", 5);
  s21_sprintf(buff2, "abc%6i", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%06i", 5);
  s21_sprintf(buff2, "abc%06i", 5);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_u) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%u", 5);
  s21_sprintf(buff2, "abc%u", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-u", 5);
  s21_sprintf(buff2, "abc%-u", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%6u", 5);
  s21_sprintf(buff2, "abc%6u", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%06u", 5);
  s21_sprintf(buff2, "abc%06u", 5);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_l) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);
  long int num = 5;
  long int num2 = -500;

  sprintf(buff1, "abc%ld", num);
  s21_sprintf(buff2, "abc%ld", num);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-ld", num);
  s21_sprintf(buff2, "abc%-ld", num);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+ld", num2);
  s21_sprintf(buff2, "abc%+ld", num2);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc% ld", num2);
  s21_sprintf(buff2, "abc% ld", num2);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%6ld", num);
  s21_sprintf(buff2, "abc%6ld", num);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%06ld", num);
  s21_sprintf(buff2, "abc%06ld", num);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_h) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%hd", 5);
  s21_sprintf(buff2, "abc%hd", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-hd", 5);
  s21_sprintf(buff2, "abc%-hd", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+hd", -100);
  s21_sprintf(buff2, "abc%+hd", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc% hd", -100);
  s21_sprintf(buff2, "abc% hd", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%6hd", 5);
  s21_sprintf(buff2, "abc%6hd", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%06hd", 5);
  s21_sprintf(buff2, "abc%06hd", 5);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_ll) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);
  long long int num = 5;
  long long int num2 = -500;

  sprintf(buff1, "abc%lld", num);
  s21_sprintf(buff2, "abc%lld", num);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%llu", num);
  s21_sprintf(buff2, "abc%llu", num);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-lld", num);
  s21_sprintf(buff2, "abc%-lld", num);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+lld", num2);
  s21_sprintf(buff2, "abc%+lld", num2);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc% lld", num2);
  s21_sprintf(buff2, "abc% lld", num2);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%6lld", num);
  s21_sprintf(buff2, "abc%6lld", num);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%06lld", num);
  s21_sprintf(buff2, "abc%06lld", num);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_hh) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%hhd", 5);
  s21_sprintf(buff2, "abc%hhd", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%hhu", 5);
  s21_sprintf(buff2, "abc%hhu", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-hhd", 5);
  s21_sprintf(buff2, "abc%-hhd", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+hhd", -100);
  s21_sprintf(buff2, "abc%+hhd", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc% hhd", -100);
  s21_sprintf(buff2, "abc% hhd", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%6hhd", 5);
  s21_sprintf(buff2, "abc%6hhd", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%06hhd", 5);
  s21_sprintf(buff2, "abc%06hhd", 5);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_percent) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%%2");
  s21_sprintf(buff2, "abc%%2");
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_o) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%o", 5);
  s21_sprintf(buff2, "abc%o", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-o", 5);
  s21_sprintf(buff2, "abc%-o", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%o", -100);
  s21_sprintf(buff2, "abc%o", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%#o", 5);
  s21_sprintf(buff2, "abc%#o", 5);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_x) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%x", 5);
  s21_sprintf(buff2, "abc%x", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-x", 5);
  s21_sprintf(buff2, "abc%-x", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%x", -100);
  s21_sprintf(buff2, "abc%x", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%#x", 5);
  s21_sprintf(buff2, "abc%#x", 5);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_X) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%X", 5);
  s21_sprintf(buff2, "abc%X", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-X", 5);
  s21_sprintf(buff2, "abc%-X", 5);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%X", -100);
  s21_sprintf(buff2, "abc%X", -100);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%#X", 5);
  s21_sprintf(buff2, "abc%#X", 5);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_pointer) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);
  int num = 5;

  sprintf(buff1, "%p", &num);
  s21_sprintf(buff2, "%p", &num);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_f) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%f", 20.02);
  s21_sprintf(buff2, "abc%f", 20.02);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%.3f", 20.0245678);
  s21_sprintf(buff2, "abc%.3f", 20.0245678);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%1.3f", 99920.0245678);
  s21_sprintf(buff2, "abc%1.3f", 99920.0245678);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-10f", 20.02);
  s21_sprintf(buff2, "abc%-10f", 20.02);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+f", -20.02);
  s21_sprintf(buff2, "abc%+f", -20.02);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_g) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%g", 20.02);
  s21_sprintf(buff2, "abc%g", 20.02);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%g", 0.0002);
  s21_sprintf(buff2, "abc%g", 0.0002);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%.gf", 20.0245678);
  s21_sprintf(buff2, "abc%.gf", 20.0245678);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%1.gf", 99920.0245678);
  s21_sprintf(buff2, "abc%1.gf", 99920.0245678);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-10g", 20.02);
  s21_sprintf(buff2, "abc%-10g", 20.02);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+g", -20.02);
  s21_sprintf(buff2, "abc%+g", -20.02);
  ck_assert_str_eq(buff1, buff2);

  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_G) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%G", 20.02);
  s21_sprintf(buff2, "abc%G", 20.02);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%G", 0.0002);
  s21_sprintf(buff2, "abc%G", 0.0002);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%.3G", 20.0245678);
  s21_sprintf(buff2, "abc%.3G", 20.0245678);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%1.3G", 99920.0245678);
  s21_sprintf(buff2, "abc%1.3G", 99920.0245678);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-10G", 20.02);
  s21_sprintf(buff2, "abc%-10G", 20.02);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+G", -20.02);
  s21_sprintf(buff2, "abc%+G", -20.02);
  ck_assert_str_eq(buff1, buff2);
  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_e) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%e", 20.02);
  s21_sprintf(buff2, "abc%e", 20.02);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%e", 0.0002);
  s21_sprintf(buff2, "abc%e", 0.0002);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%.3e", 20.0245678);
  s21_sprintf(buff2, "abc%.3e", 20.0245678);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%1.3e", 99920.0245678);
  s21_sprintf(buff2, "abc%1.3e", 99920.0245678);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-10e", 20.02);
  s21_sprintf(buff2, "abc%-10e", 20.02);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+e", -20.02);
  s21_sprintf(buff2, "abc%+e", -20.02);
  ck_assert_str_eq(buff1, buff2);
  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_sprintf_E) {
  char *buff1 = malloc(20);
  char *buff2 = malloc(20);

  sprintf(buff1, "abc%E", 20.02);
  s21_sprintf(buff2, "abc%E", 20.02);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%E", 0.0002);
  s21_sprintf(buff2, "abc%E", 0.0002);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%.3E", 20.0245678);
  s21_sprintf(buff2, "abc%.3E", 20.0245678);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%1.3E", 99920.0245678);
  s21_sprintf(buff2, "abc%1.3E", 99920.0245678);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%-10E", 20.02);
  s21_sprintf(buff2, "abc%-10E", 20.02);
  ck_assert_str_eq(buff1, buff2);

  sprintf(buff1, "abc%+E", -20.02);
  s21_sprintf(buff2, "abc%+E", -20.02);
  ck_assert_str_eq(buff1, buff2);
  free(buff1);
  free(buff2);
}
END_TEST

START_TEST(test_to_upper) {
  char *buff = s21_to_upper("aDFG0z");
  ck_assert_str_eq(buff, "ADFG0Z");
  free(buff);

  buff = s21_to_upper(NULL);
  ck_assert_ptr_null(buff);
}
END_TEST

START_TEST(test_to_lower) {
  char *buff = s21_to_lower("AZhr-r");
  ck_assert_str_eq(buff, "azhr-r");
  free(buff);

  buff = s21_to_lower(NULL);
  ck_assert_ptr_null(buff);
}
END_TEST

START_TEST(test_insert) {
  char *buff = s21_insert("aaabbb", "ccc", 3);
  ck_assert_str_eq(buff, "aaacccbbb");
  free(buff);

  buff = s21_insert("aaabbb", "ccc", 0);
  ck_assert_str_eq(buff, "cccaaabbb");
  free(buff);

  buff = s21_insert("aaa", "bbb", 3);
  ck_assert_str_eq(buff, "aaabbb");
  free(buff);

  buff = s21_insert(NULL, "ccc", 0);
  ck_assert_ptr_null(buff);

  buff = s21_insert("aaa", NULL, 0);
  ck_assert_ptr_null(buff);

  buff = s21_insert("aaa", "bbb", 4);
  ck_assert_ptr_null(buff);
}
END_TEST

START_TEST(test_trim) {
  char *buff = s21_trim("xxHello, world!xx", "!x");
  ck_assert_str_eq(buff, "Hello, world");
  free(buff);

  buff = s21_trim("abcHello, world!cba", "abc");
  ck_assert_str_eq(buff, "Hello, world!");
  free(buff);

  buff = s21_trim("  Hello, world!  ", " ");
  ck_assert_str_eq(buff, "Hello, world!");
  free(buff);

  buff = s21_trim("Hello, world!", "xyz");
  ck_assert_str_eq(buff, "Hello, world!");
  free(buff);

  buff = s21_trim("abcabc", "abc");
  ck_assert_str_eq(buff, "");
  free(buff);

  buff = s21_trim("abcabc", "");
  ck_assert_str_eq(buff, "abcabc");
  free(buff);

  buff = s21_trim(NULL, "bbb");
  ck_assert_ptr_null(buff);
}
END_TEST

START_TEST(test_sscanf_d) {
  int a1, a2;
  sscanf("123", "%d", &a1);
  s21_sscanf("123", "%d", &a2);
  ck_assert_int_eq(a1, a2);

  sscanf("asd-123bb", "asd%dbb", &a1);
  s21_sscanf("asd-123bb", "asd%dbb", &a2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_i) {
  int a1, a2;
  sscanf("123", "%i", &a1);
  s21_sscanf("123", "%i", &a2);
  ck_assert_int_eq(a1, a2);

  sscanf("asd-123bb", "asd%ibb", &a1);
  s21_sscanf("asd-123bb", "asd%ibb", &a2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_f) {
  float a1, a2;
  sscanf("123.23", "%f", &a1);
  s21_sscanf("123.23", "%f", &a2);
  ck_assert_float_eq_tol(a1, a2, 1e-6);

  sscanf("asd-123.23bb", "asd%fbb", &a1);
  s21_sscanf("asd-123.23bb", "asd%fbb", &a2);
  ck_assert_float_eq_tol(a1, a2, 1e-6);
}
END_TEST

START_TEST(test_sscanf_g) {
  float a1, a2;
  sscanf("1e3", "%g", &a1);
  s21_sscanf("1e3", "%g", &a2);
  ck_assert_float_eq_tol(a1, a2, 1e-6);

  sscanf("asd-1e3", "asd%gbb", &a1);
  s21_sscanf("asd-1e3", "asd%gbb", &a2);
  ck_assert_float_eq_tol(a1, a2, 1e-6);
}
END_TEST

START_TEST(test_sscanf_e) {
  float a1, a2;
  sscanf("1e-6", "%e", &a1);
  s21_sscanf("1e-6", "%e", &a2);
  ck_assert_float_eq_tol(a1, a2, 1e-6);

  sscanf("asd-1e-6", "asd%ebb", &a1);
  s21_sscanf("asd-1e-6", "asd%ebb", &a2);
  ck_assert_float_eq_tol(a1, a2, 1e-6);
}
END_TEST

START_TEST(test_sscanf_c) {
    char *dest1=malloc(20);
    char *dest2=malloc(20);
    
    const char *input = "test";
    
    sscanf(input, "%c", dest1);
    s21_sscanf(input, "%c", dest2);
    ck_assert_int_eq(dest1[0], dest2[0]);
    
    memset(dest1, 0, 20);
    memset(dest2, 0, 20);
    sscanf(input, "%5c", dest1);
    s21_sscanf(input, "%5c", dest2);
    ck_assert_str_eq(dest1, dest2); 

    free(dest1);
    free(dest2);
}
END_TEST

START_TEST(test_sscanf_L) {
  long double a1 = 0, a2 = 0;
  const long double epsilon = 1e-10;

  sscanf("123.01", "%Lf", &a1);
  s21_sscanf("123.01", "%Lf", &a2);
  ck_assert(fabsl(a1 - a2) < epsilon);

  sscanf("12345.01123", "%Lg", &a1);
  s21_sscanf("12345.01123", "%Lg", &a2);
  ck_assert(fabsl(a1 - a2) < epsilon);

  sscanf("50e-2", "%Le", &a1);
  s21_sscanf("50e-2", "%Le", &a2);
  ck_assert(fabsl(a1 - a2) < epsilon);
}
END_TEST

START_TEST(test_sscanf_h) {
  short a1 = 0, a2 = 0;

  sscanf("123", "%hd", &a1);
  s21_sscanf("123", "%hd", &a2);
  ck_assert_int_eq(a1, a2);

  sscanf("-32768", "%hd", &a1);
  s21_sscanf("-32768", "%hd", &a2);
  ck_assert_int_eq(a1, a2);

  sscanf("32767", "%hd", &a1);
  s21_sscanf("32767", "%hd", &a2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_l) {
  long a1 = 0, a2 = 0;

  sscanf("123456789", "%ld", &a1);
  s21_sscanf("123456789", "%ld", &a2);
  ck_assert_int_eq(a1, a2);

  sscanf("-2147483648", "%ld", &a1);
  s21_sscanf("-2147483648", "%ld", &a2);
  ck_assert_int_eq(a1, a2);

  sscanf("2147483647", "%ld", &a1);
  s21_sscanf("2147483647", "%ld", &a2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_hh) {
  signed char a1 = 0, a2 = 0;

  sscanf("123", "%hhd", &a1);
  s21_sscanf("123", "%hhd", &a2);
  ck_assert_int_eq(a1, a2);

  sscanf("-128", "%hhd", &a1);
  s21_sscanf("-128", "%hhd", &a2);
  ck_assert_int_eq(a1, a2);

  sscanf("127", "%hhd", &a1);
  s21_sscanf("127", "%hhd", &a2);
  ck_assert_int_eq(a1, a2);

  sscanf("128", "%hhd", &a1);
  s21_sscanf("128", "%hhd", &a2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_ll) {
  long long a1 = 0, a2 = 0;

  sscanf("9223372036854775807", "%lld", &a1);
  s21_sscanf("9223372036854775807", "%lld", &a2);
  ck_assert_int_eq(a1, a2);

  sscanf("-9223372036854775808", "%lld", &a1);
  s21_sscanf("-9223372036854775808", "%lld", &a2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_s) {
  char *dest1 = malloc(20);
  char *dest2 = malloc(20);

  sscanf("ahello", "a%s", dest1);
  s21_sscanf("ahello", "a%s", dest2);
  ck_assert_str_eq(dest1, dest2);

  memset(dest1, 0, 20);
  memset(dest2, 0, 20);
  s21_sscanf("abc abc2", "%s %s", dest1, dest2);
  ck_assert_str_eq(dest1, "abc");
  ck_assert_str_eq(dest2, "abc2");

  memset(dest1, 0, 20);
  memset(dest2, 0, 20);
  sscanf("test 123", "%s", dest1);
  s21_sscanf("test 123", "%s", dest2);
  ck_assert_str_eq(dest1, dest2);

  free(dest1);
  free(dest2);

  char *s1 = malloc(20);
  char *s2 = malloc(20);
  s21_sscanf("hello   world", "%s %s", s1, s2);
  ck_assert_str_eq(s1, "hello");
  ck_assert_str_eq(s2, "world");
  free(s1);
  free(s2);
}
END_TEST

START_TEST(test_sscanf_set) {
  char * dest1=malloc(20);
  char * dest2=malloc(20);
  
  sscanf("abc123", "%[a-z]", dest1);
  s21_sscanf("abc123", "%[a-z]", dest2);
  ck_assert_str_eq(dest1, dest2);

  memset(dest1, 0, 20);
  memset(dest2, 0, 20);
  sscanf("123abc", "%[^a-z]", dest1);
  s21_sscanf("123abc", "%[^a-z]", dest2);
  ck_assert_str_eq(dest1, dest2);

  memset(dest1, 0, 20);
  memset(dest2, 0, 20);
  sscanf("ABC123", "%[A-Z]", dest1);
  s21_sscanf("ABC123", "%[A-Z]", dest2);
  ck_assert_str_eq(dest1, dest2);

  memset(dest1, 0, 20);
  memset(dest2, 0, 20);
  sscanf("a1b2c3", "%[a-z0-9]", dest1);
  s21_sscanf("a1b2c3", "%[a-z0-9]", dest2);
  ck_assert_str_eq(dest1, dest2);

  memset(dest1, 0, 20);
  memset(dest2, 0, 20);
  sscanf("xyz!abc", "%[xyz!]", dest1);
  s21_sscanf("xyz!abc", "%[xyz!]", dest2);
  ck_assert_str_eq(dest1, dest2);

 char * s1=malloc(20);
 char * s2=malloc(20);
 char * s3=malloc(20);
 char * s4=malloc(20);
  
  sscanf("abc123 XYZ!", "%[a-z0-9] %[A-Z]", s1, s3);
  s21_sscanf("abc123 XYZ!", "%[a-z0-9] %[A-Z]", s2, s4);
  ck_assert_str_eq(s1, s2);
  ck_assert_str_eq(s3, s4);

  memset(dest1, 0, 20);
  memset(dest2, 0, 20);
  sscanf("abc123", "%[]]", dest1);
  s21_sscanf("abc123", "%[]]", dest2);
  ck_assert_str_eq(dest1, dest2);

  free(dest1);
  free(dest2);
  free(s1);
  free(s2);
  free(s3);
  free(s4);
}
END_TEST

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = string_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// Создание тестового набора
Suite *string_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("String");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_memchr);
  tcase_add_test(tc_core, test_memcmp);
  tcase_add_test(tc_core, test_memcpy);
  tcase_add_test(tc_core, test_memset);
  tcase_add_test(tc_core, test_strncat);
  tcase_add_test(tc_core, test_strchr);
  tcase_add_test(tc_core, test_strncmp);
  tcase_add_test(tc_core, test_strncpy);
  tcase_add_test(tc_core, test_strrchr);
  tcase_add_test(tc_core, test_strlen);
  tcase_add_test(tc_core, test_strcspn);
  tcase_add_test(tc_core, test_strpbrk);
  tcase_add_test(tc_core, test_strstr);
  tcase_add_test(tc_core, test_strtok);
  tcase_add_test(tc_core, test_strerror);

  tcase_add_test(tc_core, test_sprintf_s);
  tcase_add_test(tc_core, test_sprintf_c);
  tcase_add_test(tc_core, test_sprintf_d);
  tcase_add_test(tc_core, test_sprintf_i);
  tcase_add_test(tc_core, test_sprintf_u);
  tcase_add_test(tc_core, test_sprintf_l);
  tcase_add_test(tc_core, test_sprintf_h);
  tcase_add_test(tc_core, test_sprintf_ll);
  tcase_add_test(tc_core, test_sprintf_hh);
  tcase_add_test(tc_core, test_sprintf_percent);
  tcase_add_test(tc_core, test_sprintf_o);
  tcase_add_test(tc_core, test_sprintf_x);
  tcase_add_test(tc_core, test_sprintf_X);
  tcase_add_test(tc_core, test_sprintf_pointer);
  tcase_add_test(tc_core, test_sprintf_f);
  tcase_add_test(tc_core, test_sprintf_g);
  tcase_add_test(tc_core, test_sprintf_G);
  tcase_add_test(tc_core, test_sprintf_e);
  tcase_add_test(tc_core, test_sprintf_E);

  tcase_add_test(tc_core, test_to_upper);
  tcase_add_test(tc_core, test_to_lower);
  tcase_add_test(tc_core, test_insert);
  tcase_add_test(tc_core, test_trim);

  tcase_add_test(tc_core, test_sscanf_d);
  tcase_add_test(tc_core, test_sscanf_i);
  tcase_add_test(tc_core, test_sscanf_f);
  tcase_add_test(tc_core, test_sscanf_g);
  tcase_add_test(tc_core, test_sscanf_e);
  tcase_add_test(tc_core, test_sscanf_c);
  tcase_add_test(tc_core, test_sscanf_L);
  tcase_add_test(tc_core, test_sscanf_h);
  tcase_add_test(tc_core, test_sscanf_l);
  tcase_add_test(tc_core, test_sscanf_hh);
  tcase_add_test(tc_core, test_sscanf_ll);
  tcase_add_test(tc_core, test_sscanf_s);
tcase_add_test(tc_core, test_sscanf_set);
  suite_add_tcase(s, tc_core);

  return s;
}
