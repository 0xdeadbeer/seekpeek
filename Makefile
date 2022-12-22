GTK_FLAGS=`pkg-config --cflags gtk+-3.0`
GTK_LIBS=`pkg-config --libs gtk+-3.0`
SP_OUTPUT_DIR=build
SP_OUTPUT_NAME=seekpeek
SP_OUTPUT_LOCATION=$(SP_OUTPUT_DIR)/$(SP_OUTPUT_NAME)

run: 
	./$(SP_OUTPUT_LOCATION)

all: 
	gcc $(GTK_FLAGS) main.c -o $(SP_OUTPUT_LOCATION) $(GTK_LIBS)
	make run 
