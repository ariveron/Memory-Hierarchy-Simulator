# Commands not associated with a file
.PHONY: all setup clean rebuild run test1 test2
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
test2: all
	./bin/memory-hierarchy-simulator --c config/trace2.config --t trace/trace1.dat

# Link intermediates into executable file
bin/memory-hierarchy-simulator: int/main.o int/CommandParser.o int/TraceConfig.o int/Trace.o int/TraceLogger.o int/SwapSubject.o int/CPU.o int/TLB.o int/PT.o int/DC.o
	g++ -o bin/memory-hierarchy-simulator int/main.o int/CommandParser.o int/TraceConfig.o int/Trace.o int/TraceLogger.o int/SwapSubject.o int/CPU.o int/TLB.o int/PT.o int/DC.o

# Compile intermediate .o files for linking
int/main.o: src/main.cpp src/CommandParser.h src/TraceConfig.h src/Trace.h src/TraceLogger.h src/TLB.h src/SwapSubject.h src/PT.h src/DC.h
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
int/CPU.o: src/CPU.cpp src/CPU.h src/TraceConfig.h src/TraceLogger.h src/Trace.h src/ITranslationBuffer.h src/IPageTable.h src/IDataCache.h
	g++ -o int/CPU.o -c src/CPU.cpp
int/TLB.o: src/TLB.cpp src/TLB.h src/ITranslationBuffer.h src/TraceConfig.h src/IPageTable.h src/SwapSubject.h
	g++ -o int/TLB.o -c src/TLB.cpp
int/PT.o: src/PT.cpp src/PT.h src/IPageTable.h src/TraceConfig.h src/SwapSubject.h
	g++ -o int/PT.o -c src/PT.cpp
int/DC.o: src/DC.cpp src/PT.h src/IDataCache.h src/TraceConfig.h src/SwapSubject.h
	g++ -o int/DC.o -c src/DC.cpp

