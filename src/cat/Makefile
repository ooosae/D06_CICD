CC := gcc

CFLAGS := -std=c11 -Wall -Werror -pedantic -Wextra -Wfloat-equal -Wfloat-conversion -Wvla

OBJS := ./s21_cat.o

.PHONY : clean

s21_cat : $(OBJS)
	$(CC) -o ./$@ $^

func : s21_cat
	bash ./func_tests/scripts/func_tests.sh || echo

check:
	bash ../style.sh && rm -rf test.tmp

%.o : %.c *.h
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f ./s21_cat
	rm -f ./*.o
