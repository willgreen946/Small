CC     = cc
BIN    = tunez
LIBS   = -L/usr/local/lib
INCS   = -I/usr/local/include
LINKS  = -ldl -lpthread -lm -lSDL2
CFLAGS = -Wall -g -O3 -O
OBJS   = main.o src/miniaudio.h.gch

all: build

build: main.o
	$(CC) $(INCS) $(LIBS) $(LINKS) $(OBJS) -o $(BIN)
	mkdir -p bin
	mv $(BIN) bin/$(BIN)
	rm main.o

main.o:
	$(CC) -c $(INCS) $(LIBS) $(CFLAGS) src/main.c 

miniaudio.o:
	$(CC) -c $(CFLAGS) src/miniaudio.h

qt: 
	$(CC) $(INCS) $(LIBS) $(LINKS) src/main.c -o $(BIN) $(CFLAGS)
	mkdir -p bin
	mv $(BIN) bin/$(BIN)

run:
	./bin/$(BIN)
