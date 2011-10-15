CFLAGS=-Wall -O -std=c99

all: meta2
	./meta2 meta-ii.meta

clean:
	-rm -rf *.o
	-rm -rf meta2

%.c.h:
	gcc -g -c $< -o $@ $(CFLAGS)

meta2: meta2.o lib.o
	gcc -g -o $@ $^
