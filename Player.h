#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <iostream>
#include "Projectile.h"

class Player : public Fl_Box
{
    private:

    public:
        Player(int startX, int startY);

        Projectile* fireShot(double xVelocity, double yVelocity);
};
