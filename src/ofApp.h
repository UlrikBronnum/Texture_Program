#pragma once

#include "ofMain.h"
#include "ImageProcessing.h"
#include "GraphicMesh.h"
#include "Gui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofPixels p;

		GraphicMesh m;
		ofCamera c;
		ofMesh quad;
		ofTexture texture;

		ofVec3f		objSize;

		Gui			g1;
		Gui			g2;
		Gui			g3;
};
