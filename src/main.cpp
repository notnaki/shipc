#include "compiler/Shipc.hpp"

int main()
{
    Shipc vm("examples/01.sp");
    vm.exec();

    return 0;
}