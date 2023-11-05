#include <stdio.h>
#include "Stack/stack.h"
#include "calculator_values.h"
#include "func.h"
#include <assert.h>
#include "assembler.h"
#include "processor.h"
#include "disassembler.h"
#include <math.h>

int main(int argc, const char *argv[]) {

    maindata data = {};
    if (!MainArgs(argc, argv, &data))
        return ERROR;

    if (data.assembler) {
        if (data.asm_file_out != 0)
            CodeCompile(argv[data.asm_file_in], argv[data.asm_file_out]);
        else
            CodeCompile(argv[data.asm_file_in], byte_code);
    }

    if (data.processor) {
        Calc example = {};
        CalcCtor(&example);
        ExecuteProgram(&example, argv[data.proc_file]);
    }

    if (data.disassembler) {
        if (data.dis_file_out != 0)
            CodeTranslate(argv[data.dis_file_in], argv[data.dis_file_out]);
        else
            CodeTranslate(argv[data.dis_file_in], disassem_file);
    }

    return SUCCESS;

}
