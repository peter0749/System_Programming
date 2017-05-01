all: hw6.bin
hw6.bin: main.c 
	gcc main.c -o hw6.bin -Wall
clean:
	rm -f hw6.bin
test: hw6.bin
	./hw6.bin testdir 5 -p
