#ifndef ERROR_MACROS
#define ERROR_MACROS

void print_error(const char *error, const char *message);

#define check(x) x

#define ERROR_FAIL_COND(condition, return_value, message) \
    if (check(condition))                                 \
    {                                                     \
        print_error((const char *)return_value, message); \
        return return_value;                              \
    }                                                     \
    else                                                  \
        ((void)0)

#define ERROR_FAIL(return_value, message)             \
    print_error((const char *)return_value, message); \
    return return_value;                              \
    
#endif