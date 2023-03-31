CFLAGS=-std=c11 -g -static -Wall  # todo オプションの必要性の検証
CC = gcc
# DD = $(DEBUG)

# e:
# 	echo $(CC) $(CFLAGS) $(DD)


bin: main.o formulaParser.o bcd.o
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@

formulaParser.o: formulaParser.c
	$(CC) $(CFLAGS) $(DEBUG) -o $@ -c $<

bcd.o: bcd.c
	$(CC) $(CFLAGS) $(DEBUG) -o $@ -c $<

main.o: main.c
	$(CC) $(CFLAGS) $(DEBUG) -o $@ -c $<

run: bin
	./bin "45 + 75"

sandbox: sandbox.c
	$(CC) $(CFLAGS) $< -o $@

sandbox2: sandbox2.c
	$(CC) $(CFLAGS) $< -o $@

runs: sandbox
	./sandbox

clean: 
	rm -f bin sandbox *.o *~ tmp*

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