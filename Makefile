# Commands not associated with a file
.PHONY: all setup clean rebuild run test1
all: setup bin/memory-hierarchy-simulator
setup:
	./setup.sh
clean:
	rm -r -f bin/* int/*
rebuild: clean all
run: all
	./bin/memory-hierarchy-simulator $(ARGS)
test1: all
	./bin/memory-hierarchy-simulator --c config/trace1.config --t trace/trace1.dat

# Link intermediates into executable file
bin/memory-hierarchy-simulator: int/main.o int/CommandParser.o int/TraceConfig.o int/Trace.o int/TraceLogger.o int/SwapSubject.o
	g++ -o bin/memory-hierarchy-simulator int/main.o int/CommandParser.o int/TraceConfig.o int/Trace.o int/TraceLogger.o int/SwapSubject.o

# Compile intermediate .o files for linking
int/main.o: src/main.cpp src/CommandParser.h src/TraceConfig.h src/Trace.h src/TraceLogger.h
	g++ -o int/main.o -c src/main.cpp
int/CommandParser.o: src/CommandParser.cpp src/CommandParser.h
	g++ -o int/CommandParser.o -c src/CommandParser.cpp
int/TraceConfig.o: src/TraceConfig.cpp src/TraceConfig.h src/StringManipulation.h
	g++ -o int/TraceConfig.o -c src/TraceConfig.cpp
int/Trace.o: src/Trace.cpp src/Trace.h src/StringManipulation.h
	g++ -o int/Trace.o -c src/Trace.cpp
int/TraceLogger.o: src/TraceLogger.cpp src/TraceLogger.h src/TraceConfig.h
	g++ -o int/TraceLogger.o -c src/TraceLogger.cpp
int/SwapSubject.o: src/SwapSubject.cpp src/SwapSubject.h
	g++ -o int/SwapSubject.o -c src/SwapSubject.cpp
