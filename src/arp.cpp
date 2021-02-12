#include "arp.h"

Arp::Arp() {}

void Arp::noteOn(int note) 
{
    if (note_count < MAX_NOTES) 
    {
        note_buffer[note_count] = note;
        ++note_count;
    }
}

void Arp::noteOff(int note) 
{
    unsigned int index = 0;
    // Find the note to remove
    for (; index < note_count && note_buffer[index] != note; ++index) {}
    
    --note_count;
    // Move all following notes down
    for (; index < note_count; ++index) 
    {
        note_buffer[index] = note_buffer[index + 1];
    }
    // Erase the last one
    if (index < MAX_NOTES) 
    {
        note_buffer[index] = -1;
    }
}

void Arp::setTicksPerStep(unsigned int ticks) 
{
    ticks_per_step = ticks;
}

Arp::State Arp::getNextState() 
{
    State state;
    if (note_count == 0) {
        ticks = 0;
        return state;
    }

    if (ticks >= ticks_per_step) 
    {
        ticks = 0;
    }
    
    if (ticks == 0) {
        current_step = (current_step + 1) % note_count;
        state.cycle = State::Cycle::Trigger;
    }
    else
    {
        state.cycle = State::Cycle::Hold;
    }
    ++ticks;
    state.note = note_buffer[current_step];

    return state;
}
