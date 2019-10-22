CXX=g++
CFLAGS=-Wall
OBJ = src/main.cpp src/chip8.cpp src/machine.cpp src/display.cpp

chip8: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS) -lSDL2
