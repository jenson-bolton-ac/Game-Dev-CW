#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "StartMenu.h"
#include "InstructionsDisplay.h"

StartMenu::StartMenu(int argc, char* argv[])
	: GameSession(argc, argv)
{
	
}

StartMenu::~StartMenu(void)
{
}

void StartMenu::Start()
{
	shared_ptr<StartMenu> thisPtr = shared_ptr<StartMenu>(this);

	// Add this class as a listener of the game world
	mGameWorld->AddListener(thisPtr.get());

	// Add this as a listener to the world and the keyboard
	mGameWindow->AddKeyboardListener(thisPtr);
	mGameWindow->AddMouseListener(thisPtr);

	mWindowHeight = mGameDisplay->GetHeight();
	mWindowWidth = mGameDisplay->GetWidth();

	// Create the GUI
	CreateGUI();

	// Start the game
	GameSession::Start();
}

void StartMenu::Stop() 
{
	// Stop the game
	GameSession::Stop();
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IKeyboardListener /////////////////////

void StartMenu::OnKeyPressed(uchar key, int x, int y)
{
	switch (key)
	{
	case ' ':
		// Test functionality
		this->Stop();
		break;
	default:
		break;
	}
}

void StartMenu::OnKeyReleased(uchar key, int x, int y) {}

void StartMenu::OnSpecialKeyPressed(int key, int x, int y) {}

void StartMenu::OnSpecialKeyReleased(int key, int x, int y) {}

// PUBLIC INSTANCE METHODS IMPLEMENTING IMouseListener /////////////////////

void StartMenu::OnMouseDragged(int x, int y) {}

void StartMenu::OnMouseButton(int button, int state, int x, int y) {
	int fy = mWindowHeight - y;
	if (button == 0) {
		for (auto& b : mButtons)
			b->OnMousePressed(x, fy, button);
	}
}

void StartMenu::OnMouseMoved(int x, int y) {
	int fy = mWindowHeight - y;
	for (auto& b : mButtons) {
		int px = b->GetPosition().x - 100;
		int py = b->GetPosition().y - 20;
		int w = b->GetSize().x;
		int h = b->GetSize().y;
		bool hovered = (x >= px && x <= px + w &&
			fy >= py && fy <= py + h);
		b->SetHovered(hovered);
	}
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////

void StartMenu::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{
}

void StartMenu::CreateGUI() 
{
	mContainer = mGameDisplay->GetContainer();

	
	// Adding Main Menu ////////////////////

	mMainMenuContainer = make_shared<GUIContainer>();
	mMainMenuContainer->SetSize(GLVector2i(mWindowWidth, mWindowHeight)); // Set full screen

	struct Entry { const char* text; std::function<void()> cb; };
	vector<Entry> entries = {
		{"Start Game",    [this]() { cout << "LaunchAsteroids()"; }},
		{"Difficulty",    [this]() { cout << "ToggleDifficulty()"; }},
		{"Instructions",  [this]() 
		{
			auto win = mGameWindow; // or otherwise get your GameWindow*
			auto instrDisp = new InstructionsDisplay(win->GetDisplay()->GetWidth(),
													 win->GetDisplay()->GetHeight(),
													 win);
			win->SetDisplay(instrDisp);
		}},
		{"High Scores",   [this]() { cout << "ShowHighScores()"; }},
		{"Quit",          [this]() { this->Stop(); }}
	};

	float y = 0.8f, dy = 0.15f;
	for (auto& e : entries) {
		auto btn = make_shared<GUIButton>(e.text, e.cb);
		// center it
		btn->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
		btn->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
		// add to container at normalized (0.5, y)
		mMainMenuContainer->AddComponent(btn, GLVector2f(0.5f, y));
		mButtons.push_back(btn);
		y -= dy;
	}

	mContainer->AddComponent(mMainMenuContainer, GLVector2f(0, 0));
	mMainMenuContainer->SetVisible(true);


	// Adding Instruction Menu ////////////////////
	mInstructionsContainer = make_shared<GUIContainer>();
	mInstructionsContainer->SetSize(GLVector2i(mWindowWidth, mWindowHeight)); // Set fullscreen
	// background label
	auto bg = make_shared<GUIIcon>();
	mInstructionsContainer->AddComponent(bg, GLVector2f(0.0f, 0.0f));

	// title
	auto title = make_shared<GUILabel>("How to Play");
	title->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	title->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	title->SetPosition(GLVector2i(mWindowWidth / 2, mWindowHeight - 50));
	mInstructionsContainer->AddComponent(title, GLVector2f(0.5f, 0.9f));

	// body text
	vector<string> lines = {
		"Use arrow keys to rotate and thrust.",
		"Press space to fire bullets.",
		"Avoid and destroy all asteroids to score points.",
		"Press Esc or click BACK to return."
	};

	float y0 = 0.65f;
	dy = 0.05f;
	for (size_t i = 0; i < lines.size(); ++i) {
		auto line = make_shared<GUILabel>(lines[i]);
		line->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
		line->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
		mInstructionsContainer->AddComponent(
			line,
			GLVector2f(0.5f, y0 - i * dy)
		);
	}

	// back button
	auto backBtn = make_shared<GUIButton>("Back", [this]() { HideInstructions(); });
	backBtn->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	backBtn->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	mInstructionsContainer->AddComponent(backBtn, GLVector2f(0.5f, 0.1f));
	mButtons.push_back(backBtn);
}


// INSTRUCTION MENU ////////////////////

void StartMenu::ShowInstructions() {
	mContainer->RemoveComponent(mMainMenuContainer);
	mContainer->AddComponent(mInstructionsContainer, GLVector2f(0.0f, 0.0f));
}

void StartMenu::HideInstructions() {
	mContainer->RemoveComponent(mInstructionsContainer);
	mContainer->AddComponent(mMainMenuContainer, GLVector2f(0.0f, 0.0f));
}