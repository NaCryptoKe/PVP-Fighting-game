#ifndef INPUT_BUFFER_H
#define INPUT_BUFFER_H

#include <cstdint>

// Token identifiers for inputs worth recording (buffering, combos, specials)
enum class InputToken : std::uint8_t
{
    LIGHT_PUNCH,
    LIGHT_KICK,
    HARD_PUNCH,
    HARD_KICK,
    FIREBALL,
    THROW,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Fixed-capacity circular buffer of timestamped input events.
//
// Why a circular buffer: fighting games need a short history of inputs so a
// press made a moment too early still executes once the fighter can act
// (input buffering), and so sequences (combo links, special motions) can be
// detected afterwards. A fixed ring keeps that history allocation-free.
class InputBuffer
{
public:
    static constexpr int CAPACITY = 32;

    // Record one input event (overwrites the oldest entry when full)
    void push(InputToken token, float time)
    {
        entries[head] = Entry{ token, time, false };
        head = (head + 1) % CAPACITY;
        if (count < CAPACITY) count++;
    }

    // Removes and returns the oldest unconsumed input that is newer than
    // `maxAge` seconds. Returns false when nothing is waiting.
    bool popOldest(InputToken& outToken, float now, float maxAge)
    {
        int oldest = (head - count + CAPACITY) % CAPACITY;
        for (int i = 0; i < count; ++i)
        {
            Entry& entry = entries[(oldest + i) % CAPACITY];
            if (entry.consumed) continue;
            if (now - entry.time > maxAge) continue;   // stale, ignored

            entry.consumed = true;
            outToken = entry.token;
            return true;
        }
        return false;
    }

    // Drop every recorded input (round restarts, phase changes)
    void clear()
    {
        head = 0;
        count = 0;
    }

    int size() const { return count; }

private:
    struct Entry
    {
        InputToken token;
        float time;
        bool consumed;
    };

    Entry entries[CAPACITY] = {};   // value-initialized, oldest entries first
    int head = 0;                   // next write slot
    int count = 0;                  // number of live slots
};

#endif // INPUT_BUFFER_H
