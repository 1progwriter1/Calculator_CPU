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
	rm -rf *.dSYM

proc: $(PROC_SRC) $(LIB_SRC) $(STACK_SRC) $(VECTOR_SRC) $(LABELS_SRC)
	$(COMP) $(CFLAGS) $(PROC_SRC) $(LIB_SRC) $(STACK_SRC) $(VECTOR_SRC) $(LABELS_SRC) -o proc.out
	rm -rf *.dSYM

disasm: $(DISASM_SRC) $(LIB_SRC) $(STACK_SRC) $(VECTOR_SRC) $(LABELS_SRC)
	$(COMP) $(CFLAGS) $(DISASM_SRC) $(LIB_SRC) $(STACK_SRC) $(VECTOR_SRC) $(LABELS_SRC) -o disasm.out
	rm -rf *.dSYM

