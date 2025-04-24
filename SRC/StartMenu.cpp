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

// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////

void StartMenu::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{
}

void StartMenu::CreateGUI() 
{
	
}