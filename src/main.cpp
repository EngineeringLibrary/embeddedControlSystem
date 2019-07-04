#include "generic/output.h"

extern "C" void user_init()
{
    double matriz;

    OUTPUT<double> ch1 = new Eletrodo<double>((gpio_num_t)5);
    eletrodo->set(&matriz);
}