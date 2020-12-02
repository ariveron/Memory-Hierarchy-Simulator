# Commands not associated with a file
.PHONY: all clean run rebuild
all: bin/memory-hierarchy-simulator
clean:
	rm -r -f bin/* int/*
rebuild: clean all
run: bin/memory-hierarchy-simulator
	./bin/memory-hierarchy-simulator $(ARGS)

# Link intermediates into executable file
bin/memory-hierarchy-simulator: int/main.o int/CommandParser.o
	g++ -o bin/memory-hierarchy-simulator int/main.o int/CommandParser.o

# Create intermediate .o files for linking
int/main.o: src/main.cpp
	g++ -o int/main.o -c src/main.cpp
int/CommandParser.o: src/CommandParser.cpp src/CommandParser.h
	g++ -o int/CommandParser.o -c src/CommandParser.cpp
