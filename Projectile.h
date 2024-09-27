#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <iostream>

class Projectile : public Fl_Box
{
    private:
        //velocity in units of pixels/frame
        double xVelocity;
        double yVelocity;

        double initXVel;
        double initYVel;

        //acceleration constant (pixels/frame^2)
        static double gravAccel;

        bool enabled;

    public:
        Projectile(int startX, int startY, double initXVel, double initYVel);

        void step(double scaleFactor);
        void setEnabled(bool enabled);
        void explode(int newSize);

        bool isEnabled();
        
};

