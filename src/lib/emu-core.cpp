#include "emu-core.h"
#include <string>


void emu::throw_error (const char* e1, const char* e2) {
    std::cout << e1 << e2 << "\n";
    exit(1);
}

void emu::throw_error (const char* e1, const char* e2, const char* e3) {
    std::cout << e1 << e2 << e3 << "\n";
    exit(1);
}

void emu::throw_error (const char* e1) {
    std::cout << e1 << "\n";
    exit(1);
}