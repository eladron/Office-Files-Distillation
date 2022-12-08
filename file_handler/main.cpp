#include <iostream>
#include <cstdlib>
#define OUTPUT_DIR ("unzipped")
int main(int argc, char** argv) {
    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <Word filename>" << std::endl;
        return 1;
    }
    //extract zip file
    std::string command = "yes | unzip " + std::string(argv[1]) + " -d " + OUTPUT_DIR;
    system(command.c_str());
}
