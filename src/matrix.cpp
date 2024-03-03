#include "matrix.h"
#include "core_pins.h"

void Matrix::configure(ColumnPorts columns, RowPorts rows) {
    this->columns = columns;
    this->rows = rows;

    for (auto c : this->columns) {
        pinMode(c, OUTPUT);
        digitalWrite(c, LOW);
    }

    for (auto r : this->rows) {
        pinMode(r, INPUT);
    }
}

Matrix::Event Matrix::scan() {
    int key = 0;
    for (auto c : columns) {
        digitalWrite(c, HIGH);
        delayMicroseconds(200);
        for (auto r : rows) {
            bool pressed = digitalRead(r) == HIGH;
            if (keyState[key] != pressed) {
                keyState[key] = pressed;

                digitalWrite(c, LOW);
                return Event{key, pressed};
            }
            ++key;
        }

        digitalWrite(c, LOW);
    }
    return Event{-1, false};
}