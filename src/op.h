#ifndef AURORA_OP_H_
#define AURORA_OP_H_

#include <stdint.h>

typedef enum {
    OP_PUSH,
    OP_ADD,
} OpKind;

const char *op_kind_as_string(OpKind kind);

typedef union {
    uint32_t as_uint;
} Value;

Value uint_value(uint32_t value);

Value empty_value();

#endif // AURORA_OP_H_
