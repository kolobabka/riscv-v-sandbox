CC=gcc
CFLAGS=-march=rv64gcv

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

copy_if_less: main.o copy_if_less.o 
	$(CC) -o copy_if_less copy_if_less.o main.o 

.PHONY: clean

clean:
	rm -f ./*.o 