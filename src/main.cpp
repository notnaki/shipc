#include "compiler/Shipc.hpp"
#include <cstdio>
#include <stdexcept>
#include <string>

int main(int argc, char *argv[])
{

    if (argc == 1){
        printf("No input file provided. \n");
        return 0;
    }

    if (argc > 2){
        printf("Too many arguments in function call \n");
        return 0;
    }

    std::string file = argv[1];
    Shipc vm(file);
    vm.exec();
    return 0;
}
