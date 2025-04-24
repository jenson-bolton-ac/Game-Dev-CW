#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GUIComponent.h"
#include <functional>
#include <string>

class GUIButton : public GUIComponent
{
public:
	using Callback = std::function<void()>;

	GUIButton(const std::string& text, Callback cb);

	// override draw to render a simple rect & text
	virtual void Draw() override;

	// mouse listner callback
	virtual void OnMousePressed(int x, int y, int button);

	// hover functionality
	void SetHovered(bool h) { mHovered = h; }
	bool IsHovered() const { return mHovered; }

	void SetText(const std::string& t) { mText = t; }

private:
	std::string mText;
	Callback mCallback;
	bool mHovered;
};

#endif