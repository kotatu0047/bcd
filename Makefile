CFLAGS=-std=c11 -g -static
CC = gcc

bcd: main.o formulaParser.o
	$(CC) $(CFLAGS) $^ -o $@

sandbox: sandbox.c
	$(CC) $(CFLAGS) $< -o $@

sandbox2: sandbox2.c
	$(CC) $(CFLAGS) $< -o $@

formulaParser: formulaParser.c
	$(CC) $(CFLAGS) $< -c $@

main: main.c
	$(CC) $(CFLAGS) $< -c $@

run: bcd
	./bcd "45 + 75"

runs: sandbox
	./sandbox

clean: 
	rm -f bcd sandbox *.o *~ tmp*

.PHONY: bcd clean

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