#include "Player.h"

using namespace std;

Player::Player(int startX, int startY) : Fl_Box(startX, startY, 20, 20)
{
    this->box(FL_BORDER_BOX);
    this->color(FL_BLUE);
    cout << "player created at (" << startX << ", " << startY << ")" << endl;
}

Projectile* Player::fireShot(double xVelocity, double yVelocity)
{
    return (new Projectile(this->x() + 10, this->y(), xVelocity, yVelocity));
}
