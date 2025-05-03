#include "Asteroid.h"
#include "Asteroids.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "Spaceship.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "GUILabel.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Takes arguments from command line, just in case. */
Asteroids::Asteroids(int argc, char *argv[])
	: GameSession(argc, argv)
{
	mLevel = 0;
	mAsteroidCount = 0;
}

/** Destructor. */
Asteroids::~Asteroids(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Start an asteroids game. */
void Asteroids::Start()
{
	// Create a shared pointer for the Asteroids game object - DO NOT REMOVE
	shared_ptr<Asteroids> thisPtr = shared_ptr<Asteroids>(this);

	// Add this class as a listener of the game world
	mGameWorld->AddListener(thisPtr.get());

	// Add this as a listener to the world and the keyboard
	mGameWindow->AddKeyboardListener(thisPtr);

	/*// Add a score keeper to the game world
	mGameWorld->AddListener(&mScoreKeeper);

	// Add this class as a listener of the score keeper
	mScoreKeeper.AddListener(thisPtr);

	// Create a spaceship and add it to the world
	mGameWorld->AddObject(CreateSpaceship());
	// Create some asteroids and add them to the world
	CreateAsteroids(10);

	//Create the GUI
	CreateGUI();

	// Add a player (watcher) to the game world
	mGameWorld->AddListener(&mPlayer);

	// Add this class as a listener of the player
	mPlayer.AddListener(thisPtr);*/

	RegisterStateListener([thisPtr](GameState state) {
		switch (state) {
		case GameState::START_MENU:
			thisPtr->CreateStartMenu();
			break;
		case GameState::PLAYING:
			thisPtr->InitializeGameplay(thisPtr);
			break;
		case GameState::GAME_OVER:
			thisPtr->CreateGameOverMenu();
			break;
		case GameState::SHOWING_HIGHSCORES:
			thisPtr->ShowHighScoreTable();
			break;
		case GameState::SHOW_INSTRUCTIONS:
			thisPtr->ShowInstructions();
			break;
		default:
			break;
		}
		});

	ChangeState(GameState::START_MENU); // Start in the menu

	CreateStartMenu();

	// Start the game
	GameSession::Start();
}

/** Stop the current game. */
void Asteroids::Stop()
{
	// Stop the game
	GameSession::Stop();
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IKeyboardListener /////////////////////

void Asteroids::OnKeyPressed(uchar key, int x, int y)
{
	switch (mStateManager.GetState())
	{
	case GameState::PLAYING:
		switch (key)
		{
		case ' ':
			mSpaceship->Shoot();
			break;
		default:
			break;
		}
		break;
	
	case GameState::START_MENU:
		if (key == 13 || key == ' ') {   // Enter or Space
			ActivateMenuItem(mMenuSelection);
			return;
		}
		break;
	default: break;
	}
}

void Asteroids::OnKeyReleased(uchar key, int x, int y) {}

void Asteroids::OnSpecialKeyPressed(int key, int x, int y)
{
	switch (mStateManager.GetState())
	{
	case GameState::PLAYING:
		switch (key)
		{
			// If up arrow key is pressed start applying forward thrust
		case GLUT_KEY_UP: mSpaceship->Thrust(10); break;
			// If left arrow key is pressed start rotating anti-clockwise
		case GLUT_KEY_LEFT: mSpaceship->Rotate(90); break;
			// If right arrow key is pressed start rotating clockwise
		case GLUT_KEY_RIGHT: mSpaceship->Rotate(-90); break;
			// Default case - do nothing
		default: break;
		}
		break;

	case GameState::START_MENU:
		switch (key) {
		case GLUT_KEY_UP:
			mMenuSelection = (mMenuSelection + mMenuLabels.size() - 1)
				% mMenuLabels.size();
			UpdateMenuHighlight();
			break;
		case GLUT_KEY_DOWN:
			mMenuSelection = (mMenuSelection + 1) % mMenuLabels.size();
			UpdateMenuHighlight();
			break;
		default:
			break;
		}
		break;
	default: break;
	}
}

void Asteroids::OnSpecialKeyReleased(int key, int x, int y)
{
	if (mStateManager.GetState() != GameState::PLAYING) return;

	switch (key)
	{
	// If up arrow key is released stop applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(0); break;
	// If left arrow key is released stop rotating
	case GLUT_KEY_LEFT: mSpaceship->Rotate(0); break;
	// If right arrow key is released stop rotating
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(0); break;
	// Default case - do nothing
	default: break;
	} 
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IMouseListener /////////////////////

void Asteroids::OnMouseDragged(int x, int y)
{
}

void Asteroids::OnMouseButton(int button, int state, int x, int y)
{
	if (mStateManager.GetState() == GameState::START_MENU &&
		button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		ActivateMenuItem(mMenuSelection);
	}
}

void Asteroids::OnMouseMoved(int x, int y)
{
	if (mStateManager.GetState() != GameState::START_MENU) return;

	// Determine which label row the mouse is over by comparing y against each label's screen pos
	// (here we ask each GUILabel for its absolute position & approximate height)
	for (int i = 0; i < (int)mMenuLabels.size(); ++i) {
		auto& lbl = mMenuLabels[i];
		auto pos = lbl->GetPosition();          // from GUIComponent :contentReference[oaicite:6]{index=6}:contentReference[oaicite:7]{index=7}
		int h = lbl->GetPreferredSize().y;      // fallback, or use font height constant
		if (y >= pos.y && y <= pos.y + h) {
			if (mMenuSelection != i) {
				mMenuSelection = i;
				UpdateMenuHighlight();
			}
			break;
		}
	}
}


// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////

void Asteroids::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{
}

// PUBLIC INSTANCE METHODS IMPLEMENTING ITimerListener ////////////////////////

void Asteroids::OnTimer(int value)
{
	if (value == CREATE_NEW_PLAYER)
	{
		mSpaceship->Reset();
		mGameWorld->AddObject(mSpaceship);
	}

	if (value == START_NEXT_LEVEL)
	{
		mLevel++;
		int num_asteroids = 10 + 2 * mLevel;
	}

}

// PROTECTED INSTANCE METHODS /////////////////////////////////////////////////
shared_ptr<GameObject> Asteroids::CreateSpaceship()
{
	// Create a raw pointer to a spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mSpaceship = make_shared<Spaceship>();
	mSpaceship->SetBoundingShape(make_shared<BoundingSphere>(mSpaceship->GetThisPtr(), 4.0f));
	shared_ptr<Shape> spaceship_shape = make_shared<Shape>("spaceship.shape");
	shared_ptr<Shape> thruster_shape = make_shared<Shape>("thruster.shape");
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	mSpaceship->SetSpaceshipShape(spaceship_shape);
	mSpaceship->SetThrusterShape(thruster_shape);
	mSpaceship->SetBulletShape(bullet_shape);

	// Reset spaceship back to centre of the world
	mSpaceship->Reset();
	// Return the spaceship so it can be added to the world
	return mSpaceship;
}

void Asteroids::CreateAsteroids(const uint num_asteroids)
{
	shared_ptr<Shape> asteroid_shape = make_shared<Shape>("asteroid.shape");
	for (uint i = 0; i < num_asteroids; i++) {
		shared_ptr<GameObject> asteroid = make_shared<Asteroid>();
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 10.0f));
		asteroid->SetShape(asteroid_shape);
		mGameWorld->AddObject(asteroid);
	}
}

void Asteroids::CreateGUI()
{
	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));
	// Create a new GUILabel and wrap it up in a shared_ptr
	mScoreLabel = make_shared<GUILabel>("Score: 0");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> score_component
		= static_pointer_cast<GUIComponent>(mScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(score_component, GLVector2f(0.0f, 1.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mLivesLabel = make_shared<GUILabel>("Lives: 3");
	// Set the vertical alignment of the label to GUI_VALIGN_BOTTOM
	mLivesLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> lives_component = static_pointer_cast<GUIComponent>(mLivesLabel);
	mGameDisplay->GetContainer()->AddComponent(lives_component, GLVector2f(0.0f, 0.0f));
}

void Asteroids::OnScoreChanged(int score)
{
	// Format the score message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Score: " << score;
	// Get the score message as a string
	std::string score_msg = msg_stream.str();
	mScoreLabel->SetText(score_msg);
}

void Asteroids::OnPlayerKilled(int lives_left)
{
	// Format the lives left message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives_left;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);
}

void Asteroids::CreateStartMenu() {
	CreateAsteroids(8);

	mMenuContainer = make_shared<GUIContainer>();
	mMenuContainer->SetSize(mGameDisplay->GetContainer()->GetSize());
	mMenuContainer->SetBorder({ 50,50 });
	mGameDisplay->GetContainer()->AddComponent(mMenuContainer, { 0.0f, 0.0f });

	vector<string> entries = {
		"Game Start",
		string("Bonuses: ") + (mBonusesEnabled ? "On" : "Off"),
		"Instructions",
		"High-Scores",
		"Quit"
	};

	mMenuLabels.clear();
	for (int i = 0; i < (int)entries.size(); ++i) {
		auto lbl = make_shared<GUILabel>(entries[i]);
		lbl->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
		lbl->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
		// relative positions: x = 0.5 (centre); y = 0.7, 0.5, 0.3, 0.1
		float y = 0.7f - i * 0.1f;
		mMenuContainer->AddComponent(lbl, { 0.5f, y });
		mMenuLabels.push_back(lbl);
	}

	mMenuSelection = 0;
	UpdateMenuHighlight();
}

void Asteroids::UpdateMenuHighlight() {
	for (int i = 0; i < (int)mMenuLabels.size(); ++i) {
		// yellow for selected, white otherwise
		mMenuLabels[i]->SetColor(i == mMenuSelection
			? GLVector3f{ 1.0f,1.0f,0.0f }
		: GLVector3f{ 1.0f,1.0f,1.0f });
	}
}

void Asteroids::ActivateMenuItem(int index) {
	switch (index) {
	case 0:  // Game Start
		ChangeState(GameState::PLAYING);
		break;
	case 1:  // Toggle Bonuses
		mBonusesEnabled = !mBonusesEnabled;
		mMenuLabels[1]->SetText(
			string("Bonuses: ") + (mBonusesEnabled ? "On" : "Off"));
		UpdateMenuHighlight();
		break;
	case 2:  // Instructions
		ChangeState(GameState::SHOW_INSTRUCTIONS);
		break;
	case 3:  // High-Scores
		ChangeState(GameState::SHOWING_HIGHSCORES);
		break;
	case 4: // Quit
		this->Stop();
		break;
	}
}

void Asteroids::InitializeGameplay(shared_ptr<Asteroids> thisPtr) {
	if (mMenuContainer) {
		mGameDisplay->GetContainer()->RemoveComponent(mMenuContainer);
		mMenuContainer.reset();
		mMenuLabels.clear();
	}

	mGameWorld->AddListener(&mScoreKeeper);
	mScoreKeeper.AddListener(thisPtr);
	mGameWorld->AddObject(CreateSpaceship());
	CreateAsteroids(10);
	CreateGUI();
	mGameWorld->AddListener(&mPlayer);
	mPlayer.AddListener(thisPtr);
}

void Asteroids::CreateGameOverMenu() {
	// Add death screen
}

void Asteroids::ShowHighScoreTable() {
	// Add highscore table
}

void Asteroids::ShowInstructions() {
	// Add instuctions
}
