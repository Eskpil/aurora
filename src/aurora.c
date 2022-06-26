#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <sys/mman.h>

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

    instance->program[instance->program_size++] = inst;
}

void *instance_generate(
    AuroraInstance *instance        
) {
    uint8_t *memory = (uint8_t*) mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS ,-1, 0);

    if (memory == MAP_FAILED) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        exit(1);
    }

    size_t idx = 0;

    uint8_t raw[65535];

    while (instance->program_size > instance->ip) {
        Inst inst = instance->program[instance->ip];

        switch (inst.kind) {
            case OP_PUSH: {
                // mov
                raw[idx++] = 0xB8;
                // operand
                raw[idx++] = (inst.value.as_uint >> 0) & 0xFF;
                raw[idx++] = (inst.value.as_uint >> 8) & 0xFF;
                raw[idx++] = (inst.value.as_uint >> 16) & 0xFF;
                raw[idx++] = (inst.value.as_uint >> 24) & 0xFF;

                // push rax
                raw[idx++] = 0x50;
            } break;
            case OP_ADD: {
                // pop rax
                raw[idx++] = 0x58;

                // pop rcx
                raw[idx++] = 0x59; 

                // add rdx, rcx
                raw[idx++] = 0x48;
                raw[idx++] = 0x01;
                raw[idx++] = 0xC8;
            } break;
        }

        instance->ip++; 
    }

    raw[idx++] = 0xC3;

    memcpy(memory, &raw, idx);
    printf("Dissasembly:\t");

    for (size_t i = 0; idx > i; ++i) {
        printf("0x%x\t", memory[i]);
    }

    printf("\n");

    return (void *)memory;
}

void aurora_destroy_instance(AuroraInstance *instance)
{
    free(instance);
}

