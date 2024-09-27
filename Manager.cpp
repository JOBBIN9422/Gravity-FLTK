#include "Manager.h"

using namespace std;

Manager::Manager(int windowWidth, int windowHeight) 
: Fl_Window(windowWidth, windowHeight)
{
    srand(time(NULL));

    this->color(FL_BLACK);

    this->generateTerrain();

    this->player = new Player(20, getTerrainHeight(20) - 20);
    this->AI= new AIPlayer(760, getTerrainHeight(760) - 20);

    this->shotParamsWindow = new Fl_Window(180, 140, "Shot Parameters");
    
    this->shotAngleInput = new Fl_Int_Input(110, 20, 60, 20, "Angle");
    this->shotPowerInput = new Fl_Int_Input(110, 60, 60, 20, "Power");
    this->fireShotButton = new Fl_Button(110, 100, 60, 20, "Fire");

    ShotArgs* shotArgs = new ShotArgs();
    shotArgs->shotAngle = this->shotAngleInput;
    shotArgs->shotPower = this->shotPowerInput;
    shotArgs->fireButton = this->fireShotButton;
    shotArgs->manager = this;

    this->fireShotButton->callback(fireShotCallback, shotArgs);

    this->shotParamsWindow->show();

    this->show();
}

Player* Manager::getPlayer()
{
    return this->player;
}

Projectile* Manager::getCurrShot()
{
    return this->currentShot;
}

void Manager::setCurrShot(Projectile* shot)
{
    this->currentShot = shot;
    this->add(shot);
    Fl::add_timeout(0.0167, gameLoop, this);
}

void Manager::deleteShot()
{
    Fl::delete_widget(this->currentShot);
    this->currentShot = nullptr;
}

void Manager::generateTerrain()
{
    int terrainSize = 20;
    int numTerrainCols = this->w() / terrainSize;
    for (int i = 0; i < numTerrainCols; i++)
    {
        int columnHeight = this->terrainGenFunction(i) * terrainSize;
        //cout << i << "(" << columnHeight << ")" << endl;

        for (int j = this->h() - terrainSize; j >= this->h() - columnHeight; j -= terrainSize)
        {
            Terrain* terrain = new Terrain(i * terrainSize, j);
            this->add(terrain);
            this->terrainVector.push_back(terrain);
        }

        //cout << "height at " << i * terrainSize << ": " << getTerrainHeight(i * terrainSize) << endl << endl;
    }
}

void Manager::updateTerrainPos()
{
    int terrainSize = 20;
    int numTerrainCols = this->w() / terrainSize;

    //Iterate over each terrain column
    for (int i = 0; i < numTerrainCols; i++)
    {
        vector<Terrain*> currentCol;
        for (int j = 0; j < this->terrainVector.size() - 1; j++)
        {
            Terrain* currChunk = this->terrainVector.at(j);

            //Filter each column by finding chunks with matching x values
            if (currChunk->x() == i * terrainSize)
            {
                currentCol.push_back(currChunk);
            }
        }
        //cout << "chunk count at " << i * terrainSize << ": " << currentCol.size() << endl;
        if (!currentCol.size())
        {
            continue;
        }

        //iterate over the current x-column and check for vertical gaps
        for (int j = 0; j < currentCol.size() - 1; j++)
        {
            Terrain* currChunk = currentCol.at(j);
            Terrain* aboveChunk = currentCol.at(j + 1);

            //if there is a vertical gap in the current column, drop the chunk to fill the gap
            if (currChunk->y() - aboveChunk->y() > 20)
            {
                aboveChunk->position(aboveChunk->x(), aboveChunk->y() + 20);
            }

        }
    }
}

void Manager::checkCollisions()
{
    if (!this->currentShot)
    {
        return;
    }

    //Check shot collision with all other objects (terrain)
    for (int i = 0; i < this->terrainVector.size(); i++)
    {
        if (this->boxCollision(this->currentShot, this->terrainVector.at(i)))
        {
            //if shot hits a terrain chunk, disable shot and delete the chunk
            this->currentShot->setEnabled(false);
            Fl::delete_widget(this->terrainVector.at(i));
            this->terrainVector.erase(this->terrainVector.begin() + i);

            this->updateTerrainPos();
            return;
        }
    }
}

int Manager::terrainGenFunction(int x)
{
    //unit size of each terrain chunk
    int terrainSize = 20;
    int numTerrainCols = this->w() / terrainSize;

    //scales height of the overall terrain structure
    double heightScale = this->h() / terrainSize * 0.6;
    int squared = -1 * x + numTerrainCols / 2;

    //adds or subtracts a random amount of chunks with each function call
    int randHeightAdjustment = rand() % 3 - 1;

    //gassian function creates hill shape
    return heightScale * exp(-5 * (0.001 * (squared * squared))) + randHeightAdjustment;
}

int Manager::getTerrainHeight(int x)
{
    //holds terrain segments at the argument x value
    vector<Terrain*> argumentCol;

    //filter terrain segments based on the argument x value
    for (int i = 0; i < this->terrainVector.size(); i++)
    {
        Terrain* currChunk = this->terrainVector.at(i);

        if (currChunk->x() == x)
        {
            argumentCol.push_back(currChunk);
        }
    }

    //iterate over the requested terrain column and find chunk with the max height
    int maxHeight = argumentCol.at(0)->y();

    for (int i = 0; i < argumentCol.size(); i++)
    {
        //note that smaller y-values are considered greater altitude! 
        if (argumentCol.at(i)->y() < maxHeight)
        {
            maxHeight = argumentCol.at(i)->y();
        }
    }

    return maxHeight;
}

bool Manager::shotOutsideWindow()
{
    return (this->currentShot->x() < 0 || this->currentShot->x() > this->w() 
            || this->currentShot->y() > this->h());
}

bool Manager::boxCollision(Fl_Box* box1, Fl_Box* box2)
{
    return (box1->x() < box2->x() + box2->w() &&
            box1->x() + box1->w() > box2->x() &&
            box1->y() < box2->y() + box2->h() &&
            box1->y() + box1->h() > box2->y());
}

int Manager::handle(int event)
{
    switch (event)
    {
        //shooting
        case FL_PUSH:
            if (Fl::event_button() == FL_LEFT_MOUSE)
            {
                int playerX = this->player->x() + 20;
                int playerY = this->player->y();
                int mouseX  = Fl::event_x();
                int mouseY  = Fl::event_y();
                int xVelocity  = (mouseX - playerX) * 2;
                int yVelocity  = (mouseY - playerY) * 2;

                if (this->currentShot == nullptr)
                {
                    this->setCurrShot(player->fireShot(xVelocity, yVelocity));
                }
                return 1;
            }

        //player movement
        case FL_KEYDOWN:
            if (Fl::event_key() == 32)
            {
                this->shotParamsWindow->show();
                return 1;
            }

        default: return Fl_Widget::handle(event);
    }
}

void fireShotCallback(Fl_Widget* w, void* v)
{
    //unpack argument struct
    ShotArgs* arguments = (ShotArgs*)v;
    Manager* manager = arguments->manager;
    Fl_Int_Input* shotAngleInput = arguments->shotAngle;
    Fl_Int_Input* shotPowerInput = arguments->shotPower;
    Fl_Button* fireShotButton = arguments->fireButton;
    Player* player = manager->getPlayer();

    //fetch shot values from inputs
    int shotAngle = stoi(shotAngleInput->value()) * -1; //invert angle so positive angle is an upward arc
    int shotPower = stoi(shotPowerInput->value());

    //convert shot angle to radians 
    double pi = 3.14159;
    double shotAngleRads = shotAngle * pi / 180;

    //compute x-y velocity from angle and power
    double xVelocity = shotPower * cos(shotAngleRads);
    double yVelocity = shotPower * sin(shotAngleRads);
    cout << "X = " << xVelocity << ", Y = " << yVelocity << endl;

    manager->setCurrShot(player->fireShot(xVelocity, yVelocity));
}

void gameLoop(void* manager)
{
    Manager* game = (Manager*)manager;
    Projectile* currentShot = game->getCurrShot();

    currentShot->step(0.01);
    game->checkCollisions();

    //delete the projectile once it exits the window
    if (game->shotOutsideWindow() || !currentShot->isEnabled())
    {
        game->deleteShot();
        return;
    }
    Fl::repeat_timeout(0.0167, gameLoop, game);
}
