#include <stdio.h>
#include <stdlib.h>

#include "aurora.h"
#include "op.h"

AuroraInstance *aurora_new_instance()
{
    AuroraInstance *instance = malloc(sizeof(AuroraInstance));

    return instance;
}

void instance_push_inst(
    AuroraInstance *instance, 
    OpKind kind, 
    Value value
) {
    Inst inst = {0};
    
    inst.kind = kind;
    inst.value = value;

    instance->program[instance->ip++] = inst;
}

void aurora_destroy_instance(AuroraInstance *instance)
{
    free(instance);
}

