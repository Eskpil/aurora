#include <aurora.h>

#include <stdio.h>

typedef uint32_t (*Result)(void);

void make_simple(AuroraInstance *instance) 
{
    instance_push_inst(instance, OP_PUSH, uint_value(69));
    instance_push_inst(instance, OP_PUSH, uint_value(420));
    instance_push_inst(instance, OP_ADD, empty_value());
    instance_push_inst(instance, OP_PUSH, uint_value(100));
    instance_push_inst(instance, OP_SUB, empty_value());
    instance_push_inst(instance, OP_POP, empty_value());
}

void make_loop(AuroraInstance *instance)
{
    instance_push_inst(instance, OP_PUSH, uint_value(1));
    uint32_t addr = instance_push_inst(instance, OP_PUSH, uint_value(2));
    instance_push_inst(instance, OP_ADD, empty_value());
    instance_push_inst(instance, OP_DUP, empty_value());
    instance_push_inst(instance, OP_PUSH, uint_value(0xFF));
    instance_push_inst(instance, OP_EQ, empty_value());
    instance_push_inst(instance, OP_JMP_UNLESS, uint_value(addr));
    instance_push_inst(instance, OP_POP, empty_value());
}

extern uint32_t hello()
{
    printf("Epic\n");
    return 100;
}

void make_call(AuroraInstance *instance)
{
    instance_push_inst(instance, OP_CALL, ptr_value(&hello));
    instance_push_inst(instance, OP_POP, empty_value());
}

int main(void)
{
    AuroraInstance *instance = aurora_new_instance(); 

    make_call(instance);

    Result result = instance_generate(instance);

    uint32_t value = result();

    printf("Function returned: %ld\n", value);

    aurora_destroy_instance(instance);

    return 0; 
}
