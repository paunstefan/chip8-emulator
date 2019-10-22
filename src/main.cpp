#include <iostream>
#include <cstdlib>
#include "machine.hpp"

int main(int argc, char **argv){
	if(argc != 2){
		printf("Usage: ./chip8 [rom file]\n");
		exit(1);
	}

	chip8Machine chip8((const char*)argv[1]);

	chip8.run();
}