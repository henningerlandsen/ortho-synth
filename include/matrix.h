#include <array>
#include <bitset>

class Matrix {
public:
    struct Event {
        int key;
        bool pressed;
    };

    static constexpr int ColumnCount = 12;
    static constexpr int RowCount = 4;
    using ColumnPorts = std::array<int, ColumnCount>;
    using RowPorts = std::array<int, RowCount>; 

    void configure(ColumnPorts columns, RowPorts rows);

    Event scan();

private:
    std::bitset<ColumnCount * RowCount> keyState;

    ColumnPorts columns;
    RowPorts rows;
};