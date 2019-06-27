
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

    std::filesystem::path self_path{argv[0]};
    std::filesystem::path input_path{argv[1]};
    std::filesystem::path output_path{argv[2]};

    std::cout << input_path << " " << output_path << "\n";

    std::cout << std::filesystem::exists(self_path);

}