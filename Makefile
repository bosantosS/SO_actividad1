all: init shell_process

init: 
	mkdir -p build

shell_process: 
	gcc -o build/shell_process shell_process.c

clean:
	 rm build/*