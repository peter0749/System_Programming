all: yx2
yx2: yx2.c
	gcc yx2.c -pthread -O3 -o yx2
bench: yx2
	echo "For 1 thread:"
	./yx2 200000000 1
	echo "For 8 thread:"
	./yx2 200000000 8
clean:
	rm -f yx2

