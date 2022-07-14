DEPENDECIES= src/emu.h src/emu.cpp src/lib/* src/parser/* src/runtime/*
CVERSION =-std=c++17
all: emu
%.o: %.cpp
	g++ -c $^

emu: $(DEPENDECIES)
	g++ $(CVERSION) -o $@ $^