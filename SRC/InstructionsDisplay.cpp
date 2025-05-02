#include "InstructionsDisplay.h"
#include "GameWindow.h"

InstructionsDisplay::InstructionsDisplay(int w, int h, GameWindow* window)
    : GameDisplay(w, h), mWindow(window)
{
    BuildGUI();
}

// Build the instructions screen once:
void InstructionsDisplay::BuildGUI()
{
    auto root = GetContainer();

    // Title
    mTitle = std::make_shared<GUILabel>("How to Play");
    //mTitle->SetFontSize(24);
    //mTitle->SetColor(Color(1, 1, 1));
    mTitle->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
    root->AddComponent(mTitle, GLVector2f(0.5f, 0.85f));

    // Each line as its own label
    static const std::vector<std::string> lines = {
        "Use arrow keys to rotate and thrust.",
        "Press space to fire bullets.",
        "Avoid and destroy all asteroids to score points.",
        "Press Esc or click BACK to return."
    };
    float y0 = 0.65f, dy = 0.12f;
    for (size_t i = 0; i < lines.size(); ++i) {
        auto lbl = std::make_shared<GUILabel>(lines[i]);
        //lbl->SetFontSize(18);
        //lbl->SetColor(Color(1, 1, 1));
        lbl->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
        root->AddComponent(lbl, GLVector2f(0.5f, y0 - i * dy));
        mLines.push_back(lbl);
    }

    // Back button
    mBack = std::make_shared<GUIButton>("Back", [this]() {
        // on click, swap back to the main StartMenu display
        mWindow->SetDisplay(mWindow->GetPreviousDisplay());
        });
    // Give it a fixed size & position so its text renders correctly:
    mBack->SetSize(GLVector2i(200, 40));
    int bx = (GetWidth() - 200) / 2;
    int by = 40;
    mBack->SetPosition(GLVector2i(bx, by));
    root->AddComponent(mBack, GLVector2f(0.0f, 0.0f));
}
