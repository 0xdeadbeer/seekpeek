CFLAGS=`pkg-config --cflags gtk4 libcurl`
LIBS=`pkg-config --libs gtk4 libcurl`
SP_OUTPUT_DIR=build
SP_OUTPUT_NAME=seekpeek
SP_OUTPUT_LOCATION=$(SP_OUTPUT_DIR)/$(SP_OUTPUT_NAME)

run: 
	./$(SP_OUTPUT_LOCATION)

build: 
	mkdir -p $(SP_OUTPUT_DIR)
	gcc $(CFLAGS) main.c -o $(SP_OUTPUT_LOCATION) $(LIBS)

clean: 
	rm -rf $(SP_OUTPUT_DIR)/

all: 
	make clean
	make build 
	make run 
