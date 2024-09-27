#include "Terrain.h"

using namespace std;

Terrain::Terrain(int startX, int startY) : Fl_Box(startX, startY, 20, 20)
{
    this->box(FL_BORDER_BOX);
    this->color(FL_GREEN);
}
