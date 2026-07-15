#ifndef FLOW_HANDLE
#define FLOW_HANDLE

#include <cstdint>
#include <vector>
#include <cassert>

template <typename Tag>
struct Handle {
    uint32_t index      = UINT32_MAX;   // slot table index
    uint32_t generation = 0;
 
    bool isNull() const { return index == UINT32_MAX; }
    bool operator==(const Handle& o) const {
        return index == o.index && generation == o.generation;
    }
};

#endif