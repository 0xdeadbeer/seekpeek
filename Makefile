CFLAGS=`pkg-config --cflags gtk4 libcurl`
LIBS=`pkg-config --libs gtk4 libcurl`
SINGLE_LIBS=-llexbor_static
SP_OUTPUT_DIR=build
SP_OUTPUT_NAME=seekpeek
SP_OUTPUT_LOCATION=$(SP_OUTPUT_DIR)/$(SP_OUTPUT_NAME)
SOURCES=$(shell find . -name '*.c')
DEBUG_FLAGS=-g3

run: 
	./$(SP_OUTPUT_LOCATION)
SOURCES=$(wildcard *.c)

%.o: %.c
	gcc -c $(CFLAGS) -o $(SP_OUTPUT_DIR)/$(notdir $@) $^ $(LIBS) $(SINGLE_LIBS)
#	gcc $(CFLAGS) main.c -o $(SP_OUTPUT_LOCATION) $(LIBS) $(SINGLE_LIBS)

build: 
	mkdir $(SP_OUTPUT_DIR)
	gcc $(CFLAGS) -o $(SP_OUTPUT_LOCATION) $(shell find . -name '*.c') $(LIBS) $(SINGLE_LIBS)

debug_build: 
	mkdir $(SP_OUTPUT_DIR)
	gcc $(CFLAGS) $(DEBUG_FLAGS) -o $(SP_OUTPUT_LOCATION) $(shell find . -name '*.c') $(LIBS) $(SINGLE_LIBS)

clean: 
	rm -rf $(SP_OUTPUT_DIR)/

all: 
	make clean
	make build 
	make run 

debug: 
	make clean 
	make debug_build 
	make run 