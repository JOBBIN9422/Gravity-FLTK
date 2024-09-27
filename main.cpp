#include "Manager.h"
#include <iostream>

using namespace std;

int main()
{
    Manager* manager = new Manager(800, 600);

    return Fl::run();
}
