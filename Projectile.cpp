#include "Projectile.h"

using namespace std;

double Projectile::gravAccel = 25;

Projectile::Projectile(int startX, int startY,  double initXVel, double initYVel) 
: Fl_Box(startX, startY, 10, 10), xVelocity(initXVel), yVelocity(initYVel), 
initXVel(initXVel), initYVel(initYVel), enabled(true)
{
    this->box(FL_ROUND_UP_BOX);
    this->color(FL_GRAY);
}

void Projectile::step(double scaleFactor)
{
    if (!this->enabled)
    {
        return;
    }
    //cout << this->xVelocity * scaleFactor << " " << this->yVelocity * scaleFactor << endl;
    //calculate the new coordinates based on velocity
    int newX = this->x() + scaleFactor * this->xVelocity;
    int newY = this->y() + scaleFactor * this->yVelocity;

    //move the projectile to its newly calculated position (must hide and show)
    this->hide();
    this->position(newX, newY);
    this->show();
    this->redraw();

    //update y velocity due to gravity (x velocity is constant)
    this->yVelocity += gravAccel;
}

void Projectile::setEnabled(bool enabled)
{
    this->enabled = enabled;
    if (!enabled)
    {
        this->hide();
    }
}

void Projectile::explode(int newSize)
{
    this->box(FL_OFLAT_BOX);
    this->w(newSize);
    this->h(newSize);
    this->color(FL_YELLOW);
}

bool Projectile::isEnabled()
{
    return this->enabled;
}
