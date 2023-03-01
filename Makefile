CFLAGS=-std=c11 -g -static
CC = gcc

main: main.c
	$(CC) $(CFLAGS) $< -o $@

sandbox: sandbox.c
	$(CC) $(CFLAGS) $< -o $@

run: main
	./main

runs: sandbox
	./sandbox

clean: 
	rm -f main *.o *~ tmp*

.PHONY: main clean

# gcc_options = -std=c++17 -Wall --pedantic-errors

# program : main.c
#     g++ $(gcc_options) -include all.h $< -o $@

# all.h.gch : all.h
#     g++ $(gcc_options) -x c++-header -o $@ $<

# run : program
#     ./program

# clean :
#     rm -f ./program
#     rm -f ./all.h.gch

# .PHONY : run clean