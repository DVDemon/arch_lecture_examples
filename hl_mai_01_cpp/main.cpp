#include <iostream>
#include <exception>

#include "config.h"
#include "new_lib.h"

int main()
{
    std::cout << "Hello world!" << std::endl;
    std::cout << get_config_string() << std::endl;
    my_func(123);
    std::cout << "result:" << complex_calculation(2,2) << std::endl;
    return 0;
}