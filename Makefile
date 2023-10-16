all:
	g++ -c main.cpp func.cpp assembler.cpp disassembler.cpp processor.cpp Stack/stack.cpp Stack/hash.cpp
	g++ main.o func.o assembler.o disassembler.o processor.o stack.o hash.o
	mv main.o outputfiles/main.o
	mv func.o outputfiles/func.o
	mv assembler.o outputfiles/assembler.o
	mv disassembler.o outputfiles/disassembler.o
	mv processor.o outputfiles/processor.o
	mv stack.o outputfiles/stack.o
	mv hash.o outputfiles/hash.o

calc:
	g++ -c main.cpp func.cpp assembler.cpp disassembler.cpp processor.cpp
	g++ main.o func.o assembler.o disassembler.o outputfiles/stack.o outputfiles/hash.o processor.o
	mv main.o outputfiles/main.o
	mv func.o outputfiles/func.o
	mv assembler.o outputfiles/assembler.o
	mv disassembler.o outputfiles/disassembler.o
	mv processor.o outputfiles/processor.o

comp:
	g++ -c main.cpp func.cpp assembler.cpp disassembler.cpp processor.cpp