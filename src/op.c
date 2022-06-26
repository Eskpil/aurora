#include "op.h"

const char *op_kind_as_string(OpKind kind)
{
    switch (kind) {
        case OP_PUSH: return "OP_PUSH";
        case OP_ADD: return "OP_ADD";
        default: return "unreachable";
    };
}

Value uint_value(uint32_t data) 
{
    Value value = {0};
    value.as_uint = data;
    return value; 
}

Value empty_value() 
{
    Value value = {0};
    return value;
}
