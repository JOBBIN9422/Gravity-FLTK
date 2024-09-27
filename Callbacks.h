struct ShotArgs
{
    Manager* manager;
    Fl_Int_Input* shotAngle;
    Fl_Int_Input* shotPower;

    Fl_Button* fireButton;
};

void fireShotCallback(Fl_Widget* w, void* v);
