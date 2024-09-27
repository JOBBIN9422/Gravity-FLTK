#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Int_Input.H>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "Projectile.h"
#include "Player.h"
#include "AIPlayer.h"
#include "Terrain.h"

class Manager : public Fl_Window
{
    private:
        //keep track of human and AI player
        Player* player;
        AIPlayer* AI;

        //keep track of the active shot
        Projectile* currentShot;

        std::vector<Terrain*> terrainVector;

        Fl_Window* shotParamsWindow; //sets power and angle of player shot
        
        //shot parameters window
        Fl_Button* fireShotButton; //fires a shot with the provided settings

        Fl_Int_Input* shotAngleInput;
        Fl_Int_Input* shotPowerInput;

    public:
        Manager(int windowWidth, int windowHeight);

        Player* getPlayer();
        Projectile* getCurrShot();

        int handle(int event);
        int terrainGenFunction(int x);
        int getTerrainHeight(int x);
        
        bool shotOutsideWindow();
        bool boxCollision(Fl_Box* box1, Fl_Box* box2);

        void setCurrShot(Projectile* shot);
        void deleteShot();
        void generateTerrain();
        void updateTerrainPos();

        void checkCollisions();
};

struct ShotArgs
{
    Manager* manager;
    Fl_Int_Input* shotAngle;
    Fl_Int_Input* shotPower;

    Fl_Button* fireButton;
};

void fireShotCallback(Fl_Widget* w, void* v);

//main loop logic, called with repeated timeouts at 60fps
void gameLoop(void* manager);
