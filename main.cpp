
#include <iostream>
#include <string>
#include <filesystem>

int main(int argc, char* argv[]) {
#ifdef __arm__
    std::cout << "ARM\n";
#endif

    if(argc != 3) {
        std::cout << "Usage: ./ctr input_file output_file\n";
        return 1;
    }

    std::filesystem::path input_path{argv[1]};
    std::filesystem::path output_path{argv[2]};

    if(!std::filesystem::exists(input_path)) {
        std::cout << input_path << " \x1B[31mnot found in\033[0m " << std::filesystem::current_path() << "\n";
    }

    if(!std::filesystem::exists(output_path)) {
        std::cout << output_path << " \x1B[31mnot found in\033[0m " << std::filesystem::current_path() << "\n";
    }

    if(!std::filesystem::exists(input_path) || !std::filesystem::exists(output_path)) {
        return 1;
    }

    std::cout << input_path << " " << output_path << "\n";

}