#ifndef FLOW_POOL
#define FLOW_POOL

#include "core/handle.h"

template <typename T, typename Tag>
class Pool
{
public:
    using H = Handle<Tag>;

    H create(const T &value)
    {
        uint32_t slot;
        if (freeHead_ != UINT32_MAX)
        {
            // Reuse a free slot. Its generation was already
            // bumped at destroy time, so old handles to it
            // are permanently dead.
            slot = freeHead_;
            freeHead_ = slots_[slot].nextFree;
        }
        else
        {
            slot = uint32_t(slots_.size());
            slots_.push_back({});
        }

        // New object always goes at the END of the dense array:
        // creation never disturbs existing packing.
        const uint32_t denseIndex = uint32_t(data_.size());
        data_.push_back(value);
        denseToSlot_.push_back(slot);

        slots_[slot].denseIndex = denseIndex;
        slots_[slot].alive = true;

        return {slot, slots_[slot].generation};
    }

    void destroy(H h)
    {
        assert(isValid(h));
        Slot &s = slots_[h.index];

        const uint32_t hole = s.denseIndex;
        const uint32_t last = uint32_t(data_.size()) - 1;

        if (hole != last)
        {
            // Move the LAST element into the hole...
            data_[hole] = std::move(data_[last]);
            denseToSlot_[hole] = denseToSlot_[last];
            // ...and repoint the moved element's slot entry.
            // Its handles still work: their slot index didn't
            // change, only where that slot points.
            slots_[denseToSlot_[hole]].denseIndex = hole;
        }
        data_.pop_back();
        denseToSlot_.pop_back();

        // Kill this incarnation and recycle the slot.
        s.generation++; // all old handles now stale
        s.alive = false;
        s.nextFree = freeHead_;
        freeHead_ = h.index;
    }

    bool isValid(H h) const
    {
        return h.index < slots_.size() && slots_[h.index].alive && slots_[h.index].generation == h.generation;
    }

    T &get(H h)
    {
        assert(isValid(h));
        return data_[slots_[h.index].denseIndex];
    }
    T *tryGet(H h)
    {
        return isValid(h) ? &data_[slots_[h.index].denseIndex]
                          : nullptr;
    }

    T *data() { return data_.data(); }
    const T *data() const { return data_.data(); }
    uint32_t count() const { return uint32_t(data_.size()); }

    H handleOf(uint32_t denseIndex) const
    {
        const uint32_t slot = denseToSlot_[denseIndex];
        return {slot, slots_[slot].generation};
    }

private:
    struct Slot
    {
        uint32_t denseIndex = 0; // where the data lives NOW
        uint32_t generation = 0; // incarnation counter
        uint32_t nextFree = UINT32_MAX;
        bool alive = false;
    };

    std::vector<T> data_;               // dense, contiguous, hot
    std::vector<uint32_t> denseToSlot_; // parallel: dense -> slot
    std::vector<Slot> slots_;           // sparse, stable
    uint32_t freeHead_ = UINT32_MAX;
};

#endif