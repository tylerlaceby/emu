#include "emu-core.h"
#include <string>
#include "colors.h"


void emu::error (const char* e1, const char* e2) {
    printf(KRED); 
    std::cout << e1 << e2 << "\n";
    printf(RST);
    exit(1);
}

void emu::error (const char* e1, const char* e2, const char* e3) {
    printf(KRED); 
    std::cout << e1 << e2 << e3 << "\n";
    printf(RST);
    exit(1);
}

void emu::error (const char* e1) {
    printf(KRED); 
    std::cout << e1 << "\n";
    printf(RST);
    exit(1);
}

void emu::runtime_exception (const char* e1) {
    printf(KRED);
    std::cout << "\nEmu Runtime Exception: \n";
    std::cout << e1 << "\n";
    printf(RST);
    exit(1);
}

void emu::runtime_exception (const char* e1, const char* e2) {
    printf(KRED);
    std::cout << "\nEmu Runtime Exception: \n";
    std::cout << e1 <<  e2 <<  "\n";
    printf(RST);
    exit(1);
}