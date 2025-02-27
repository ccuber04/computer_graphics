#include <iostream>

#include "random.h"

int main() {
    int n = 10000;
    double avg = 0;
    for (int i = 0; i < n; i++) {
        // avg += random_double();
        avg += random_double(10, 20);
    }
    std::cout << avg / n << '\n';
    return 0;
}