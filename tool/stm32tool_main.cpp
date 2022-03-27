#include "stm32tool.h"

int main(int argc, const char **argv)
{
    SerialConfig config("COM4", 115200, 8, NO, 1);

    Stm32tool stm32tool(config);

    return stm32tool.run();
}
