#ifndef __ASTEROIDS_H__
#define __ASTEROIDS_H__

#include "GameUtil.h"
#include "GameSession.h"
#include "GameStateManager.h"
#include "GUIContainer.h"
#include "IKeyboardListener.h"
#include "IMouseListener.h"
#include "IGameWorldListener.h"
#include "IScoreListener.h" 
#include "ScoreKeeper.h"
#include "Player.h"
#include "IPlayerListener.h"

class GameObject;
class Spaceship;
class GUILabel;

class Asteroids : public GameSession, public IKeyboardListener, IMouseListener, public IGameWorldListener, public IScoreListener, public IPlayerListener
{
public:
	Asteroids(int argc, char *argv[]);
	virtual ~Asteroids(void);

	virtual void Start(void);
	virtual void Stop(void);

	// Declaration of IKeyboardListener interface ////////////////////////////////

	void OnKeyPressed(uchar key, int x, int y);
	void OnKeyReleased(uchar key, int x, int y);
	void OnSpecialKeyPressed(int key, int x, int y);
	void OnSpecialKeyReleased(int key, int x, int y);

	// Declaration of IMouseListener interface ////////////////////////////////

	void OnMouseDragged(int x, int y);
	void OnMouseButton(int button, int state, int x, int y);
	void OnMouseMoved(int x, int y);

	// Declaration of IScoreListener interface //////////////////////////////////

	void OnScoreChanged(int score);

	// Declaration of the IPlayerLister interface //////////////////////////////

	void OnPlayerKilled(int lives_left);

	// Declaration of IGameWorldListener interface //////////////////////////////

	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);

	// Override the default implementation of ITimerListener ////////////////////
	void OnTimer(int value);

	// Implementation of GameStateManager listeners ////////////////////

	int RegisterStateListener(const GameStateManager::Listener& listener) { return mStateManager.AddListener(listener); }
	void UnregisterStateListener(int listenerId) { mStateManager.RemoveListener(listenerId); }

private:

	// Handle the game state
	void ChangeState(GameState newState) {
		mStateManager.SetState(newState);
	}

	GameStateManager mStateManager;

	// Changing game states
	void CreateStartMenu();
	void InitializeGameplay(shared_ptr<Asteroids> this_ptr);
	void CreateGameOverMenu();
	void ShowHighScoreTable();
	void ShowInstructions();

	shared_ptr<Spaceship> mSpaceship;
	shared_ptr<GUILabel> mScoreLabel;
	shared_ptr<GUILabel> mLivesLabel;

	// Game over
	string mCurrentTag;
	shared_ptr<GUILabel> mTagLabel;
	static const size_t MAX_TAG_LEN = 12;
	int mScore;

	uint mLevel;
	uint mAsteroidCount;

	void ResetSpaceship();
	shared_ptr<GameObject> CreateSpaceship();
	void CreateGUI();
	void CreateAsteroids(const uint num_asteroids);
	shared_ptr<GameObject> CreateExplosion();
	
	const static uint SHOW_GAME_OVER = 0;
	const static uint START_NEXT_LEVEL = 1;
	const static uint CREATE_NEW_PLAYER = 2;

	ScoreKeeper mScoreKeeper;
	Player mPlayer;

	// Menu handling ////////////////////

	vector<shared_ptr<GameObject>> asteroids;

	shared_ptr<GUIContainer> mMenuContainer;
	vector<shared_ptr<GUILabel>> mMenuLabels;
	int mMenuSelection = 0;
	bool mBonusesEnabled = true;

	void UpdateMenuHighlight(); // Highlight all labels based on mMenuSelection
	void ActivateMenuItem(int index); // Activate the currently selected menu item
	vector<pair<string, int>> LoadHighScores() const; // Load the highscore
};

#endif