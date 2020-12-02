# Commands not associated with a file
.PHONY: all clean rebuild run test1
all: bin/memory-hierarchy-simulator
clean:
	rm -r -f bin/* int/*
rebuild: clean all
run: bin/memory-hierarchy-simulator
	./bin/memory-hierarchy-simulator $(ARGS)
test1: bin/memory-hierarchy-simulator
	./bin/memory-hierarchy-simulator --c config/trace1.config --t trace/trace1.dat

# Link intermediates into executable file
bin/memory-hierarchy-simulator: int/main.o int/CommandParser.o
	g++ -o bin/memory-hierarchy-simulator int/main.o int/CommandParser.o

# Create intermediate .o files for linking
int/main.o: src/main.cpp src/CommandParser.h
	g++ -o int/main.o -c src/main.cpp
int/CommandParser.o: src/CommandParser.cpp src/CommandParser.h
	g++ -o int/CommandParser.o -c src/CommandParser.cpp
