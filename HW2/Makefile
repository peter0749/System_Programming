all: fib fibpp
fibpp: fib.cpp
	g++ fib.cpp -o fibpp
fib: fib.c
	gcc fib.c -o fib
clean:
	rm -f fib
	rm -f fibpp
test: fib
	./fib 0 1 91
