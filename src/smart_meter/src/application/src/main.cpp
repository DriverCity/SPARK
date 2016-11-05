#include <iostream>
#include "Logger/Logger.h"

int main()
{
    spark::Logger::init();
    LOG_DEBUG("Hello World!");
    return 0;
}
