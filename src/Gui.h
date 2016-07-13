#pragma once

#include "ofMain.h"
#include "GraphicMesh.h"

class Gui
{
public:
	Gui();
	~Gui();

	void setup(	int eWidth, int eHeight, 
				ofVec2f texCoord1 = ofVec2f(0.0f, 0.0f), ofVec2f texCoord2 = ofVec2f(0.0f, 1.0f), 
				ofVec2f texCoord3 = ofVec2f(1.0f, 0.0f), ofVec2f texCoord4 = ofVec2f(1.0f, 1.0f));

	void draw(int x, int y, ofTexture& t1, ofTexture& t2);

	GraphicMesh		mesh;

protected:

	int				posX;
	int				posY;

	int				width;
	int				height;

	

	ofTexture*		diffuse;
	ofTexture*		normalmap;

};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------


class GuiElement
{
public:
	GuiElement();
	~GuiElement();


	void enableEvents();
	void diableEvents();

	// Mouse interactions
	virtual bool mouseMoved(ofMouseEventArgs & args) { return false; };
	virtual bool mousePressed(ofMouseEventArgs & args) { return false; };
	virtual bool mouseDragged(ofMouseEventArgs & args) { return false; }
	virtual bool mouseReleased(ofMouseEventArgs & args) { return false; };

	// Key interactions
	virtual bool keyPressed(ofKeyEventArgs & args) { return false; };
	virtual bool keyReleased(ofKeyEventArgs & args) { return false; };

protected:

	string			name;
	bool            state;

	bool            mouseEnable;
	bool            keyEnable;

	
	// Mouse activate events
	void registerMouseEvent();
	void unregisterMouseEvent();

	// Key activate events
	void registerKeyEvent();
	void unregisterKeyEvent();


};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------