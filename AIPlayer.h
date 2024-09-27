#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <iostream>
#include <cstdlib>
#include "Projectile.h"

class AIPlayer : public Fl_Box
{
    private:
        int currShotAngle;
        int currShotPower;

    public:
        AIPlayer(int startX, int startY);

        Projectile* fireShot(double xVelocity, double yVelocity);
};
