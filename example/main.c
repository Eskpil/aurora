#include <aurora.h>

typedef uint32_t (*Result)(void);

int main(void)
{
    AuroraInstance *instance = aurora_new_instance(); 

    Result result;

    instance_push_inst(instance, OP_PUSH, uint_value(69));
    instance_push_inst(instance, OP_PUSH, uint_value(420));
    instance_push_inst(instance, OP_ADD, empty_value());
    instance_push_inst(instance, OP_PUSH, uint_value(100));
    instance_push_inst(instance, OP_SUB, empty_value());
    instance_push_inst(instance, OP_POP, empty_value());

    result = instance_generate(instance);

    uint32_t value = result();

    printf("Function returned: %ld\n", value);

    aurora_destroy_instance(instance);

    return 0; 
}
