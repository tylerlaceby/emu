#include "./emu-core.h"
#include <fstream>
#include <streambuf>

std::string emu::io::read_file_to_string (const char* filename) {
    std::ifstream t(filename);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}