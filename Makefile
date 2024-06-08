CFLAGS=-ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla -DNDEBUG -Ofast
# CFLAGS=-DNDEBUG -Ofast -ggdb3 -std=c++17

COMP=g++

OBJ_DIR=outputfiles/
OBJECTS=$(wildcard *.o)

LIB_DIR=src/lib
STACK_DIR=src/stack
VECTOR_DIR=src/vector
LABELS_DIR=src/labels

ASM_DIR=src/asm
PROC_DIR=src/proc
DISASM_DIR=src/disasm

LIB_SRC=$(wildcard $(LIB_DIR)/*.cpp)
STACK_SRC=$(wildcard $(STACK_DIR)/*.cpp)
VECTOR_SRC=$(wildcard $(VECTOR_DIR)/*.cpp)
LABELS_SRC=$(wildcard $(LABELS_DIR)/*.cpp)

ASM_SRC=$(wildcard $(ASM_DIR)/*.cpp)
PROC_SRC=$(wildcard $(PROC_DIR)/*.cpp)
DISASM_SRC=$(wildcard $(DISASM_DIR)/*.cpp)

asm: $(ASM_SRC) $(LIB_SRC) $(STACK_SRC) $(VECTOR_SRC) $(LABELS_SRC)
	$(COMP) $(CFLAGS) $(ASM_SRC) $(LIB_SRC) $(STACK_SRC) $(VECTOR_SRC) $(LABELS_SRC) -o asm.out

proc: $(PROC_SRC) $(LIB_SRC) $(STACK_SRC) $(VECTOR_SRC) $(LABELS_SRC)
	$(COMP) $(CFLAGS) $(PROC_SRC) $(LIB_SRC) $(STACK_SRC) $(VECTOR_SRC) $(LABELS_SRC) -o proc.out

disasm: $(DISASM_SRC) $(LIB_SRC) $(STACK_SRC) $(VECTOR_SRC) $(LABELS_SRC)
	$(COMP) $(CFLAGS) $(DISASM_SRC) $(LIB_SRC) $(STACK_SRC) $(VECTOR_SRC) $(LABELS_SRC) -o disasm.out

# comp:
# 	$(COMP) $(CFLAGS) -c func.cpp assembler.cpp disassembler.cpp processor.cpp my_vector.cpp labels.cpp asm.cpp disasm.cpp proc.cpp Stack/stack.cpp Stack/hash.cpp
#
# move:
# 	mv $(OBJECTS) $(OBJ_DIR)/
#
# run:
# 	$(COMP) $(CFLAGS) valgrind --tool=callgrind func.o assembler.o disassembler.o outputfiles/stack.o outputfiles/hash.o processor.o my_vector.o labels.o
# 	mv labels.o outputfiles/labels.o
# 	mv func.o outputfiles/func.o
# 	mv my_vector.o outputfiles/my_vector.o
# 	mv assembler.o outputfiles/assembler.o
# 	mv disassembler.o outputfiles/disassembler.o
# 	mv processor.o outputfiles/processor.o
#
# proc:
# 	$(COMP) $(CFLAGS) -c proc.cpp processor.cpp
# 	$(COMP) proc.o processor.o outputfiles/func.o outputfiles/stack.o outputfiles/hash.o -o proc.out
# 	mv proc.o outputfiles/proc.o
# 	mv processor.o outputfiles/processor.o
#
# asm:
# 	$(COMP) $(CFLAGS) -c asm.cpp assembler.cpp
# 	$(COMP) asm.o assembler.o outputfiles/func.o outputfiles/my_vector.o outputfiles/labels.o outputfiles/stack.o outputfiles/hash.o  -o asm.out
# 	mv asm.o outputfiles/asm.o
# 	mv assembler.o outputfiles/assembler.o
#
# disasm:
# 	$(COMP) $(CFLAGS) -c disasm.cpp disassembler.cpp
# 	$(COMP) disasm.o disassembler.o outputfiles/func.o outputfiles/my_vector.o outputfiles/stack.o outputfiles/hash.o -o disasm.out
# 	mv disasm.o outputfiles/disasm.o
# 	mv disassembler.o outputfiles/disassembler.o
#
# full: proc asm disasm

