CC := gcc

CFLAGS := -std=c11 -Wall -Werror -pedantic -Wextra -Wfloat-equal -Wfloat-conversion -Wvla

OBJS := ./s21_grep.c

.PHONY : clean

s21_grep : $(OBJS)
	$(CC) -o ./$@ $^
	
func : s21_grep
	bash ./func_tests/scripts/func_tests.sh || echo

check:
	bash ../style.sh && rm -rf test.tmp

%.o : %.c *.h
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f ./s21_grep
	rm -f ./*.o
