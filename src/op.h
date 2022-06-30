#ifndef AURORA_OP_H_
#define AURORA_OP_H_

#include <stdint.h>

typedef enum {
    OP_PUSH = 0,
    OP_DUP,

    OP_ADD,
    OP_MUL,
    
    OP_JMP,
    OP_BRK,
    OP_JMP_UNLESS,

    OP_CALL,

    OP_EQ,

    OP_SUB,
    OP_POP,
} OpKind;

const char *op_kind_as_string(OpKind kind);

typedef union {
    uint32_t as_uint;
    void *as_ptr;
} Value;

Value uint_value(uint32_t value);
Value ptr_value(void *data);

Value empty_value();

#endif // AURORA_OP_H_
