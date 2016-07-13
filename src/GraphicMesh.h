#pragma once

#include "ofMain.h"

class GraphicMesh 
{
public:
	GraphicMesh();
	~GraphicMesh();

	void draw(ofTexture& t1, ofTexture& t2);
	void draw();

	void createMesh(ofMesh& m);
	void createQuad(int w, int h, ofVec2f texCoord1 = ofVec2f(0.0f,0.0f) , ofVec2f texCoord2 = ofVec2f(1.0f, 0.0f), ofVec2f texCoord3 = ofVec2f(0.0f, 1.0f), ofVec2f texCoord4 = ofVec2f(1.0f, 1.0f));

	void loadShader(string vertexShader, string fragmentShader);



	void setLightPos(ofVec4f pos) { lightPos = pos; }
	
	ofVec3f getCenter() { return mesh.getCentroid(); }

	

protected:

	void calcNormals();
	void calcTangents();

	ofVec4f			lightPos;

	ofShader		shader;
	ofVboMesh		mesh;
	
	vector<ofVec3f> tangents;
};

