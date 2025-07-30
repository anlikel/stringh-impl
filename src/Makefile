# Makefile

# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -I.
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -lcheck -lsubunit -lm -lpthread

# Directories
TEST_DIR = test

all: s21_string.a test gcov_report

s21_string.a: s21_string_1.o s21_sprintf_1.o s21_sscanf_1.o s21_common_1.o
	ar rcs $@ $^
	ranlib $@

s21_string_1.o: s21_string.c
	$(CC) $(CFLAGS) -c $< -o $@

s21_sprintf_1.o: s21_sprintf.c
	$(CC) $(CFLAGS) -c $< -o $@
	
s21_sscanf_1.o: s21_sscanf.c
	$(CC) $(CFLAGS) -c $< -o $@
	
s21_common_1.o: s21_common.c
	$(CC) $(CFLAGS) -c $< -o $@

test.exe: test.o s21_string.o s21_sprintf.o s21_sscanf.o s21_common.o
	$(CC) $^ -o $@ $(LDFLAGS) $(COVERAGE_FLAGS)
	
test.o: test.c s21_string.h
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c $< -o $@

s21_sscanf.o: s21_sscanf.c s21_string.h s21_common.h
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c $< -o $@

s21_sprintf.o: s21_sprintf.c s21_string.h s21_common.h
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c $< -o $@
	
s21_common.o: s21_common.c s21_common.h s21_string.h
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c $< -o $@
	
s21_string.o: s21_string.c s21_string.h s21_error_desc.h
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c $< -o $@

.PHONY: test clean rebuild gcov_report

test: test.exe
	./test.exe

gcov_report: test.exe
	@mkdir -p $(TEST_DIR)
	./test.exe
	gcov -b -c -f s21_string.gcda s21_sprintf.gcda s21_sscanf.gcda s21_common.gcda
	lcov --capture --directory . --output-file $(TEST_DIR)/coverage.info --rc branch_coverage=1
	genhtml $(TEST_DIR)/coverage.info --output-directory $(TEST_DIR) --branch-coverage
	@echo "Отчет сформирован в папке $(TEST_DIR)/index.html"

clean:
	rm -rf *.o *.exe *.gcda *.gcno *.gcov *.a
	rm -rf $(TEST_DIR)

rebuild: clean all
