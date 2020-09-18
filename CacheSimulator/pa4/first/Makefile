all: first

first: first.c
	gcc -g -lm -Wall -Werror -fsanitize=address -std=c11 first.c -o first

clean:
	rm -f first
