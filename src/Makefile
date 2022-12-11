CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11 -g
GCOV = --coverage
TEST = smartcalc_test
STLIB = s21_smartcalc.a
OS := $(shell uname -s)

ifeq ($(OS), Linux)
	LIBS := -lcheck_pic -lpthread -lrt -lm -lsubunit -g
endif
ifeq ($(OS), Darwin)
	LIBS := -lcheck
endif

all: clean gcov_report dist dvi

install: clean
	mkdir build
	cd s21_smartcalc && qmake && make && make clean
ifeq ($(OS), Linux)
	mv s21_smartcalc/s21_smartcalc build
	cd build && ./s21_smartcalc
endif
ifeq ($(OS), Darwin)
	mv s21_smartcalc/s21_smartcalc.app build
	cd build && ./s21_smartcalc.app
endif

uninstall:
	rm -rf build tgz_smartcalc.tgz

clean: uninstall
	rm -rf *.o *.a *.gc* test.info info.pdf info.dvi info.log info.aux report $(TEST) tgz_smartcalc .clang-format s21_smartcalc/.clang-format RESULT_VALGRIND.txt build* s21_smartcalc/s21_smartcalc s21_smartcalc/s21_smartcalc.app s21_smartcalc/Makefile s21_smartcalc/*.o

dvi:
	texi2pdf info.tex
	texi2dvi info.tex

dist: install
	mkdir tgz_smartcalc/ tgz_smartcalc/src
	mv ./build/s21_smartcalc tgz_smartcalc/src/
	tar cvzf tgz_smartcalc.tgz tgz_smartcalc/

tests: clean
	$(CC) -c s21_smartcalc.c
	ar rcs $(STLIB) s21_smartcalc.o
	ranlib $(STLIB)
	$(CC) $(FLAGS) smartcalc_test.c $(STLIB) -o $(TEST) $(LIBS)
	./$(TEST)

check: tests
	cp ../materials/linters/.clang-format . && cp ../materials/linters/.clang-format s21_smartcalc/
	clang-format -n *.c *.h
	cd s21_smartcalc && clang-format -n  *.ui *.c* *.h
	cppcheck --suppress=missingIncludeSystem *.c
ifeq ($(OS), Linux)
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./$(TEST)
else
	CK_FORK=no leaks --atExit -- ./$(TEST)
endif

gcov_report: tests
	$(CC) $(FLAGS) $(GCOV) smartcalc_test.c s21_smartcalc.c -o $(TEST) $(LIBS)
	./$(TEST)
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/src/index.html