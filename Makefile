CC=gcc

CFLAGS=-Wall -Iinclude

LDFLAGS = -lX11 -lpthread
BIN=bin

all: $(BIN)/launcher \
     $(BIN)/window \
     $(BIN)/ialearner

$(BIN)/launcher: src/launcher.c
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) src/launcher.c -o $(BIN)/launcher

$(BIN)/window: src/window.c src/socket_utils.c
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) src/window.c src/socket_utils.c -o $(BIN)/window $(LDFLAGS)

$(BIN)/ialearner: src/ialearner.c src/classifier.c
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) src/ialearner.c src/classifier.c -o $(BIN)/ialearner -lpthread

clean:
	rm -rf $(BIN)
