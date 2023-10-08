BIN=exe

all:
	mkdir -p objs
	c++ -c -g -O3 src/main.cpp -o objs/main.o
	mkdir -p asm
	c++ -S -O3 src/main.cpp -o asm/main.s
	mkdir -p bin
	c++ objs/main.o -o bin/$(BIN)
run:
	./bin/exe
view:
	objdump -d -Mintel bin/$(BIN) | less
clean:
	rm -r bin
	rm -r objs
	rm -r asm
