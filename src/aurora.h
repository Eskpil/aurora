#ifndef AURORA_AURORA_H_
#define AURORA_AURORA_H_

#include "op.h"

#include <stdlib.h>

typedef struct {
    OpKind kind;
    Value value;
} Inst;

typedef struct {
    Inst program[65535];
    size_t ip;
    size_t program_size;
} AuroraInstance;

AuroraInstance *aurora_new_instance();

void instance_push_inst(AuroraInstance *instance, OpKind kind, Value value);

void *instance_generate(AuroraInstance *instance);

void aurora_destroy_instance(AuroraInstance *instance);

#endif // AURORA_AURORA_H_
