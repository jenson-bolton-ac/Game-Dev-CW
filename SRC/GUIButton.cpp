#include "GUIButton.h"
#include "GUILabel.h"
#include "GameUtil.h"

GUIButton::GUIButton(const std::string& text, Callback cb)
	: mText(text), mCallback(cb), mHovered(false)
{
	// default centred text
	SetHorizontalAlignment(GUI_HALIGN_CENTER);
	SetVerticalAlignment(GUI_VALIGN_MIDDLE);
	// default size
	SetSize(GLVector2i(200, 40));
}

void GUIButton::Draw() 
{
	if (!GetVisible()) return;

	// draw background
	if (mHovered) {
		glColor3f(0.8f, 0.8f, 1.0f); // temp blue
	}
	else {
		glColor3f(0.2f, 0.2f, 0.2f); // default grey
	}

	// compute rectangle corners
	int x = mPosition.x-100, y = mPosition.y-20;
	int w = mSize.x, h = mSize.y;
	glBegin(GL_QUADS);
	  glVertex2i(x, y);
	  glVertex2i(x + w, y);
	  glVertex2i(x + w, y + h);
	  glVertex2i(x, y + h);
	glEnd();

	// draw border
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	  glVertex2i(x, y);
	  glVertex2i(x + w, y);
	  glVertex2i(x + w, y + h);
	  glVertex2i(x, y + h);
	glEnd();

	// draw label centred
	glColor3f(1, 1, 1);
	int textWidth = (int)mText.length() * 9;   // match GUILabel font
	int textHeight = 15;
	int tx = x + (w - textWidth) / 2;
	int ty = y + (h + textHeight - 20) / 2;            // baseline
	glRasterPos2i(tx, ty);
	for (char c : mText) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}

void GUIButton::OnMousePressed(int mx, int my, int button) {
	// left button only
	if (button != 0) return;

	// hit test
	if (mx >= mPosition.x - 100 && mx <= mPosition.x - 100 + mSize.x &&
		my >= mPosition.y - 20 && my <= mPosition.y - 20 + mSize.y)
	{
		if (mCallback) mCallback();
	}
}