CC = gcc

CFLAGS = -O2 -std=c99 -pedantic -Wall -o -lm

scan: mainScan.c scanner.c
	$(CC) $(CFLAGS) $^ -o $@

recog: mainRecog.c scanner.c recognizeEq.c
	$(CC) $(CFLAGS) $^ -o $@

eval: scanner.c recognizeEq.c evalExp.c mainEvalEq.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean debug-scan

clean:
	rm -f eval recog scan

debug-scan: scan
	cat example_part1_input.txt | valgrind ./scan
