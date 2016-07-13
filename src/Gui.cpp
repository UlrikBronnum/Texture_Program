#include "Gui.h"



Gui::Gui()
{
}


Gui::~Gui()
{
}

void Gui::setup(int eWidth, int eHeight, ofVec2f texCoord1, ofVec2f texCoord2, ofVec2f texCoord3, ofVec2f texCoord4)
{
	width = eWidth;
	height = eHeight;

	mesh.createQuad(width, height, texCoord1, texCoord2, texCoord3, texCoord4);
	//mesh.loadShader("res/shaders/defaultVertex.vert", "res/shaders/meshColor.frag");
}
void Gui::draw(int x, int y, ofTexture& t1, ofTexture& t2)
{
	ofPushMatrix();

	ofTranslate(x + width/2, y + height/2);
	mesh.draw(t1,t2);

	ofPopMatrix();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

GuiElement::GuiElement()
{
}


GuiElement::~GuiElement()
{
}

void GuiElement::enableEvents()
{
	if (mouseEnable)
		registerMouseEvent();

	if (keyEnable)
		registerKeyEvent();

}
void GuiElement::diableEvents()
{
	if (mouseEnable)
		unregisterMouseEvent();

	if (keyEnable)
		unregisterKeyEvent();
}

// Mouse activate events
void GuiElement::registerMouseEvent()
{
	ofAddListener(ofEvents().mouseMoved, this, &GuiElement::mouseMoved);
	ofAddListener(ofEvents().mousePressed, this, &GuiElement::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &GuiElement::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &GuiElement::mouseReleased);
}
void GuiElement::unregisterMouseEvent()
{
	ofRemoveListener(ofEvents().mouseMoved, this, &GuiElement::mouseMoved);
	ofRemoveListener(ofEvents().mousePressed, this, &GuiElement::mousePressed);
	ofRemoveListener(ofEvents().mouseDragged, this, &GuiElement::mouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &GuiElement::mouseReleased);
}

// Key activate events
void GuiElement::registerKeyEvent()
{
	ofAddListener(ofEvents().keyPressed, this, &GuiElement::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &GuiElement::keyReleased);
}
void GuiElement::unregisterKeyEvent()
{
	ofRemoveListener(ofEvents().keyPressed, this, &GuiElement::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &GuiElement::keyReleased);
}

