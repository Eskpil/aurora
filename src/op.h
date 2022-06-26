#ifndef AURORA_OP_H_
#define AURORA_OP_H_

#include <stdint.h>

typedef enum {
    OP_PUSH,
    OP_ADD,
} OpKind;

typedef union {
    uint64_t as_uint;
} Value;

Value uint_value(uint64_t value);

Value empty_value();

#endif // AURORA_OP_H_
