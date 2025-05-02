#ifndef __STARTMENU_H__
#define __STARTMENU_H__

#include "GameUtil.h"
#include "GameSession.h"
#include "GUIButton.h"
#include "GUIContainer.h"
#include "IMouseListener.h"
#include <vector>

class StartMenu : public GameSession, public IKeyboardListener, public IMouseListener, public IGameWorldListener
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

	// Declaration of IMouseListener interface ////////////////////////////////

	void OnMouseDragged(int x, int y);
	void OnMouseButton(int button, int state, int x, int y);
	void OnMouseMoved(int x, int y);

	// Declaration of IGameWorldListener interface //////////////////////////////

	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);

private:
	int mWindowHeight;
	int mWindowWidth;

	vector<shared_ptr<GUIButton>> mButtons;
	GUIContainer* mContainer;

	void CreateGUI();
};

#endif