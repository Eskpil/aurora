#include "op.h"

Value uint_value(uint64_t data) 
{
    Value value = {0};
    value.as_uint = data;
    return value; 
}

Value empty_value() 
{
    Value value = {0};
    return value;
}
