#include <iostream>

int main(int argc, char* argv[]) {
    for(auto i{0u}; i < argc; ++i) {
        std::cout << i << " " << argv[i] << std::endl;
    }
}