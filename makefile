DEPENDECIES= src/emu.h src/emu.cpp src/lib/* src/parser/* src/runtime/*
CVERSION =-std=c++17
CPLER=g++
all: emu
%.o: %.cpp
	$(CPLER) -c $^

emu: $(DEPENDECIES)
	$(CPLER) $(CVERSION) -o $@ $^