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

    instance->program_size = 0;

    instance->code[instance->program_size++] = 0x41;
    instance->code[instance->program_size++] = 0x5a;

    instance->memory = mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (instance->memory == MAP_FAILED) {
        fprintf(stderr, "Failed to allocate executable memory: %s\n", strerror(errno));
        exit(1);
    }

    return instance;
}

uint32_t instance_push_inst(
    AuroraInstance *instance, 
    OpKind kind, 
    Value value
) {
    Inst inst = {0};
    
    inst.kind = kind;
    inst.value = value;

    uint32_t start = instance->program_size;
    
    printf("Generating: %s\n", op_kind_as_string(inst.kind));

    switch (inst.kind) {
        case OP_PUSH: {
            printf("\tValue: %d\n", inst.value.as_uint);
            // mov
            instance->code[instance->program_size++] = 0xB8;
            // operand
            instance->code[instance->program_size++] = (inst.value.as_uint >> 0) & 0xFF;
            instance->code[instance->program_size++] = (inst.value.as_uint >> 8) & 0xFF;
            instance->code[instance->program_size++] = (inst.value.as_uint >> 16) & 0xFF;
            instance->code[instance->program_size++] = (inst.value.as_uint >> 24) & 0xFF;

            // push rax
            instance->code[instance->program_size++] = 0x50;
        } break;
        case OP_DUP: {
            // pop rax
            instance->code[instance->program_size++] = 0x58;

            // push rax (x2)
            instance->code[instance->program_size++] = 0x50;
            instance->code[instance->program_size++] = 0x50;
        } break;
        case OP_ADD: {
            // pop rax
            instance->code[instance->program_size++] = 0x58;

            // pop rcx
            instance->code[instance->program_size++] = 0x59; 

            // add rax, rcx
            instance->code[instance->program_size++] = 0x48;
            instance->code[instance->program_size++] = 0x01;
            instance->code[instance->program_size++] = 0xC8;

            // push rax
            instance->code[instance->program_size++] = 0x50; 
        } break;
        case OP_SUB: {
            // pop rcx
            instance->code[instance->program_size++] = 0x59;
            
            // pop rax 
            instance->code[instance->program_size++] = 0x58;      

            // sub rax, rcx
            instance->code[instance->program_size++] = 0x48;
            instance->code[instance->program_size++] = 0x29;
            instance->code[instance->program_size++] = 0xC8;

            // push rax
            instance->code[instance->program_size++] = 0x50;
        } break;
        case OP_MUL: {
            // pop rcx
            instance->code[instance->program_size++] = 0x59;

            // pop rax
            instance->code[instance->program_size++] = 0x58;      

            // mul rax, rcx
            instance->code[instance->program_size++] = 0x48;
            instance->code[instance->program_size++] = 0xF7;
            instance->code[instance->program_size++] = 0xC8;

            // push rax
            instance->code[instance->program_size++] = 0x50;
        } break;
                    
        case OP_EQ: {
            // pop rax (lhs of op)     
            instance->code[instance->program_size++] = 0x58;

            // pop rcx (rhs of op)
            instance->code[instance->program_size++] = 0x59;

            // cmp rax, rcx
            instance->code[instance->program_size++] = 0x48;
            instance->code[instance->program_size++] = 0x39;
            instance->code[instance->program_size++] = 0xC8;

            // sete al
            instance->code[instance->program_size++] = 0x0F;
            instance->code[instance->program_size++] = 0x94;
            instance->code[instance->program_size++] = 0xC0; 

            // moxzx rax, al
            instance->code[instance->program_size++] = 0x48;
            instance->code[instance->program_size++] = 0x0F;
            instance->code[instance->program_size++] = 0xB6;
            instance->code[instance->program_size++] = 0xC0;

            // push rax
            instance->code[instance->program_size++] = 0x50;
        } break;
        
        case OP_JMP: {
            printf("\tAddr: %d\n", inst.value.as_uint);      
        } break;
        case OP_BRK: {

        } break;
        case OP_JMP_UNLESS: {
            printf("\tAddr: 0x%x (%d)\n", inst.value.as_uint, inst.value.as_uint);
            // pop rax
            instance->code[instance->program_size++] = 0x58;

            // test rax, rax
            instance->code[instance->program_size++] = 0x48;
            instance->code[instance->program_size++] = 0x85;
            instance->code[instance->program_size++] = 0xC0;

            uint32_t target = (((uint64_t )instance->memory) - (inst.value.as_uint >> 0) & 0xFF) - 32;
            // uint32_t target = (((uint64_t) instance->memory) - (instance->program_size)) + inst.value.as_uint;

            printf("\tTarget: 0x%x (%d)\n", target, target);

            // je {addr}
            instance->code[instance->program_size++] = 0x74;
            instance->code[instance->program_size++] = (target >> 0) & 0xFF;
            // instance->code[instance->program_size++] = (inst.value.as_uint >> 0) & 0xFF;
        } break;

        case OP_CALL: {
            uint64_t addr = (uint64_t) inst.value.as_ptr;                      

            // mov rax, {addr}
            instance->code[instance->program_size++] = 0x48;
            instance->code[instance->program_size++] = 0xB8;

            instance->code[instance->program_size++] = (addr >> 0) & 0xFF;
            instance->code[instance->program_size++] = (addr >> 8) & 0xFF;
            instance->code[instance->program_size++] = (addr >> 16) & 0xFF;
            instance->code[instance->program_size++] = (addr >> 24) & 0xFF;
            instance->code[instance->program_size++] = (addr >> 32) & 0xFF;
            instance->code[instance->program_size++] = (addr >> 40) & 0xFF;
            instance->code[instance->program_size++] = (addr >> 48) & 0xFF;
            instance->code[instance->program_size++] = (addr >> 54) & 0xFF;

            // call rax
            instance->code[instance->program_size++] = 0xFF;
            instance->code[instance->program_size++] = 0xD0;

            // push rax
            instance->code[instance->program_size++] = 0x50; 
        } break;

        case OP_POP: {
            // pop rax
            instance->code[instance->program_size++] = 0x58;
        } break;
    }


    return start;
}

void *instance_generate(
    AuroraInstance *instance        
) {
    uint8_t *memory = (uint8_t*) mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS ,-1, 0);

    if (memory == MAP_FAILED) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        exit(1);
    }

    printf("Address: 0x%p\n", memory);

    // push r10 (ret addr we saved earlier)
    instance->code[instance->program_size++] = 0x41;
    instance->code[instance->program_size++] = 0x52;
    
    //ret
    instance->code[instance->program_size++] = 0xC3;

    memcpy(memory, &instance->code, instance->program_size);
    printf("\nDissasembly:\n\n");

    for (size_t i = 0; instance->program_size > i; ++i) {
        printf("\t%-3x: ", i);
        if (memory[i] == 0xB8) {
            printf("0x%x 0x%x 0x%x 0x%x 0x%x\n", memory[i++], memory[i++], memory[i++], memory[i++], memory[i]);
        } else if (memory[i] == 0x48) {
            printf("0x%x ", memory[i++]);
            if (memory[i] == 0x0F) {
                printf("0x%x 0x%x 0x%x\n", memory[i++], memory[i++], memory[i]);   
            } else if (memory[i] == 0xBF) {
                printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", memory[i++], memory[i++], memory[i++], memory[i++], memory[i++], memory[i++], memory[i++], memory[i++], memory[i]);
            } else if (memory[i] == 0xB8) {
                printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", memory[i++], memory[i++], memory[i++], memory[i++], memory[i++], memory[i++], memory[i++], memory[i++], memory[i]);
            } else {
                printf("0x%x 0x%x\n", memory[i++], memory[i]); 
            }
        } else if (memory[i] == 0xFF) {
            printf("0x%x 0x%x\n", memory[i++], memory[i]);    
        } else if (memory[i] == 0x0F) {
            printf("0x%x 0x%x 0x%x\n", memory[i++], memory[i++], memory[i]);
        } else if (memory[i] == 0x74) {
            printf("0x%x 0x%x\n", memory[i++], memory[i]);
        } else if (memory[i] == 0x41) {
            printf("0x%x 0x%x\n", memory[i++], memory[i]);
        } else {
            printf("0x%x\n", memory[i]);
        }
    }

    printf("\n");

    return (void *)memory;
}

void aurora_destroy_instance(AuroraInstance *instance)
{
    free(instance);
}

