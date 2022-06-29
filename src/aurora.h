#ifndef AURORA_AURORA_H_
#define AURORA_AURORA_H_

#include "op.h"

#include <stdlib.h>

typedef struct {
    OpKind kind;
    Value value;
} Inst;

typedef struct {
    size_t program_size;
    uint8_t code[65535];

    void *memory;
} AuroraInstance;

AuroraInstance *aurora_new_instance();

uint32_t instance_push_inst(AuroraInstance *instance, OpKind kind, Value value);

void *instance_generate(AuroraInstance *instance);

void aurora_destroy_instance(AuroraInstance *instance);

#endif // AURORA_AURORA_H_
