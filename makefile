DEPENDECIES= src/emu.h src/emu.cpp src/lib/* src/analysis/* src/bytecode-vm/* src/parser/* src/runtime/*
CVERSION =-std=c++20
all: emu
%.o: %.cpp
	g++ -c $^

emu: $(DEPENDECIES)
	g++ $(CVERSION) -o $@ $^