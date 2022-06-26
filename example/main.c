#include <aurora.h>

int main(void)
{
    AuroraInstance *instance = aurora_new_instance(); 

    instance_push_inst(instance, OP_PUSH, uint_value(69));
    instance_push_inst(instance, OP_PUSH, uint_value(420));
    instance_push_inst(instance, OP_ADD, empty_value());

    aurora_destroy_instance(instance);

    return 0; 
}
