#include "./emu-core.h"
#include <fstream>
#include <streambuf>

std::string emu::io::read_file_to_string (const char* filename) {
    std::string file = std::string(std::string(std::filesystem::current_path()) + "/" +  filename);
    if (!std::filesystem::exists(filename) || !std::filesystem::is_regular_file(file))
        emu::error("Cannot locate emu file. Please make sure file path is valid: ", file.c_str());
    
    std::ifstream t(file);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}