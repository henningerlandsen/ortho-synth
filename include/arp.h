class Arp {
public:
    Arp();

    void noteOn(int note);

    void noteOff(int note);

    void setTicksPerStep(unsigned int ticks);

    struct State {
        enum Cycle {
            Trigger,
            Hold,
            Release,
            Off
        };
        int note = -1;
        Cycle cycle = Cycle::Off;
    };

    State getNextState();

private:
    static constexpr unsigned int MAX_NOTES = 6u;
    int note_buffer[MAX_NOTES] = { -1, -1, -1, -1, -1, -1 };
    unsigned int note_count = 0;
    unsigned int ticks_per_step = 10000;
    unsigned int current_step = 0;
    unsigned int ticks = 0;
};