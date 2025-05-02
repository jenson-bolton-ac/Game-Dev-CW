#ifndef __INSTRUCTIONSDISPLAY_H__
#define __INSTRUCTIONSDISPLAY_H__

#include "GameDisplay.h"
#include "GUIButton.h"
#include "GUILabel.h"
#include "GameUtil.h"
#include <vector>

class GameWindow;

class InstructionsDisplay : public GameDisplay
{
public:
    InstructionsDisplay(int w, int h, GameWindow* window);
    virtual ~InstructionsDisplay() {}

    // override nothing else

private:
    GameWindow* mWindow;
    shared_ptr<GUILabel>  mTitle;
    vector<shared_ptr<GUILabel>> mLines;
    shared_ptr<GUIButton> mBack;

    void BuildGUI();
};

#endif
