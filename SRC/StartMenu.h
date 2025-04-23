#ifndef __STARTMENU_H__
#define __STARTMENU_H__

#include "GameUtil.h"
#include "GameSession.h"

class StartMenu : public GameSession, public IKeyboardListener, public IGameWorldListener
{
public:
	StartMenu(int argc, char* argv[]);
	virtual ~StartMenu(void);

	virtual void Start(void);
	virtual void Stop(void);

	// Declaration of IKeyboardListener interface ////////////////////////////////

	void OnKeyPressed(uchar key, int x, int y);
	void OnKeyReleased(uchar key, int x, int y);
	void OnSpecialKeyPressed(int key, int x, int y);
	void OnSpecialKeyReleased(int key, int x, int y);

	// Declaration of IGameWorldListener interface //////////////////////////////

	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);

private:
	void CreateGUI();
};

#endif