
#include <iostream>
#include <string>
#include <filesystem>

namespace fs =  std::filesystem;

int main(int argc, char* argv[]) {
#ifdef __arm__
    std::cout << "ARM\n";
#endif

    if(argc != 3) {
        std::cout << "Usage: ./ctr input_file output_file\n";
        return 1;
    }

    fs::path input_path{argv[1]};
    fs::path output_path{argv[2]};

    if(!fs::exists(input_path)) {
        std::cout << input_path << " \033[1;31mnot found in\033[0m " << fs::current_path() << "\n";
        return 1;
    }

    std::cout << fs::file_size(input_path) << "\n";

}