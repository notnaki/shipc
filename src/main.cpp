#include "compiler/Shipc.hpp"

int main()
{
    Shipc vm("examples/00.sp");
    vm.exec();

    return 0;
}