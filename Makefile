CC = gcc
NOWARN = -Wno-parentheses -Wno-sign-compare -Wno-unused-result
CFLAGS = -std=c89 -O4 -Wall -Werror -Wextra -pedantic $(NOWARN)
LDFLAGS = -lm -lGL -lGLU -lglut
IDIRS = include
MODULES = cam kbd
MODULES_BIN = $(patsubst %, bin/%.o, $(MODULES))

all: main

main: src/main.c $(MODULES_BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(IDIRS) -o $@ $^

bin/%.o: src/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(IDIRS) -c -o $@ $<

install:
	@echo "make: install not supported"

clean:
	rm -f main $(MODULES_BIN);
