#include <iostream>

int main(int argc, char* argv[]) {
    if(int a = 5; a > 0) {
        std::cout << "C++17" << std::endl;
    }
    for(int i{0}; i < argc; ++i) {
        std::cout << i << " " << argv[i] << std::endl;
    }
}