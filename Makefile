all:
	g++ -c main.cpp func.cpp translator.cpp disassembler.cpp calculate.cpp Stack/stack.cpp Stack/hash.cpp
	g++ main.o func.o translator.o disassembler.o calculate.o stack.o hash.o

calc:
	g++ -c main.cpp func.cpp translator.cpp disassembler.cpp calculate.cpp
	g++ main.o func.o translator.o disassembler.o stack.o hash.o calculate.o
