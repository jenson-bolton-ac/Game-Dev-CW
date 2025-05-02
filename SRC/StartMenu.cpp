#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "StartMenu.h"

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
	
	struct Entry { const char* text; std::function<void()> cb; };
	vector<Entry> entries = {
		{"Start Game",    [this]() { cout << "LaunchAsteroids()"; }},
		{"Difficulty",    [this]() { cout << "ToggleDifficulty()"; }},
		{"Instructions",  [this]() { cout << "ShowInstructions()"; }},
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
		mContainer->AddComponent(btn, GLVector2f(0.5f, y));
		mButtons.push_back(btn);
		y -= dy;
	}
}