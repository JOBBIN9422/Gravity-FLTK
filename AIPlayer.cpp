#include "AIPlayer.h"

using namespace std;

AIPlayer::AIPlayer(int startX, int startY) : Fl_Box(startX, startY, 20, 20)
{
    this->box(FL_BORDER_BOX);
    this->color(FL_RED);

    this->currShotAngle = rand() % 41 + 20;
    this->currShotPower = rand() % 1001 + 500;

    //cout << currShotAngle << " " << currShotPower << endl;
    cout << "AI player created at (" << startX << ", " << startY << ")" << endl;
}

Projectile* AIPlayer::fireShot(double xVelocity, double yVelocity)
{
    return (new Projectile(this->x() + 20, this->y(), xVelocity, yVelocity));
}
