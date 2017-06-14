all: pi
pi: pi.c
	gcc pi.c -pthread -O3 -o pi
bench: pi
	echo "base:"
	./pi 20000000 1
	echo "parallel:"
	./pi 20000000 4
clean:
	rm -f pi
