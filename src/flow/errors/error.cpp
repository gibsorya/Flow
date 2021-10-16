#include "error.hpp"

const char *errors[] = {
    "SUCCESS",
    "FAILED",
    "CANNOT CREATE",
    "INVALID VALUE/COMMAND",
    "NOT FOUND",
};

static_assert(sizeof(errors) / sizeof(*errors) == MAX);