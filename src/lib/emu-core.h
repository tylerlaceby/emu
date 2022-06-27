#ifndef EMU_CORE_LIBRARY
#define EMU_CORE_LIBRARY

#include <string>
#include <iostream>
#include <stdio.h>
#include "colors.h"

// EMU Language core library and helper methods.
namespace emu {
    // Throws a error and quit the emu process.
    void throw_error (const char* e1, const char* e2, const char* e3);
    void throw_error (const char* e1, const char* e2);
    void throw_error (const char* e1);

    // Handles common IO operations.
    namespace io {
        // Read a file contents to a string.
        std::string read_file_to_string (const char* filename);
    };

};

static void repeatPrint (char c, int space) {
    for (int i = 0; i < space; i++)
        printf("%c", c);
}



#endif