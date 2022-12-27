GTK_FLAGS=`pkg-config --cflags gtk4`
GTK_LIBS=`pkg-config --libs gtk4`
SP_OUTPUT_DIR=build
SP_OUTPUT_NAME=seekpeek
SP_OUTPUT_LOCATION=$(SP_OUTPUT_DIR)/$(SP_OUTPUT_NAME)

run: 
	./$(SP_OUTPUT_LOCATION)

build: 
	mkdir -p $(SP_OUTPUT_DIR)
	gcc $(GTK_FLAGS) main.c -o $(SP_OUTPUT_LOCATION) $(GTK_LIBS)

clean: 
	rm -rf $(SP_OUTPUT_DIR)/

all: 
	make clean
	make build 
	make run 
