#include "op.h"

const char *op_kind_as_string(OpKind kind)
{
    switch (kind) {
        case OP_PUSH: return "OP_PUSH";
        case OP_DUP: return "OP_DUP";

        case OP_ADD: return "OP_ADD";
        case OP_SUB: return "OP_SUB";
        case OP_MUL: return "OP_MUL";

        case OP_JMP: return "OP_JMP";
        case OP_BRK: return "OP_BRK";
        case OP_JMP_UNLESS: return "OP_JMP_UNLESS";

        case OP_CALL: return "OP_CALL";

        case OP_EQ: return "OP_EQ";

        case OP_POP: return "OP_POP";
        default: return "unreachable";
    };
}

Value uint_value(uint32_t data) 
{
    Value value = {0};
    value.as_uint = data;
    return value; 
}

Value ptr_value(void *data)
{
    Value value = {0};
    value.as_ptr = data;
    return value;
}

Value empty_value() 
{
    Value value = {0};
    return value;
}
