#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableDepthTest();
	ofDisableArbTex();

	objSize = ofVec3f(300,300,1);

	ofLoadImage(p, "images/diff.jpg");
	p.resize(512, 512);

	//m.createMesh(ofMesh::plane(objSize.x, objSize.y,2,2, OF_PRIMITIVE_TRIANGLES));//sphere( 250 , 48 , OF_PRIMITIVE_TRIANGLES));//OF_PRIMITIVE_TRIANGLES
	//m.createMesh(ofMesh::box(objSize.x, objSize.y, objSize.z));//sphere( 250 , 48 , OF_PRIMITIVE_TRIANGLES));//OF_PRIMITIVE_TRIANGLES
	m.createQuad(objSize.x, objSize.y);
	c.setGlobalPosition(m.getCenter() + ofVec3f(0, 0, objSize.z/2.0f + 250));
	c.lookAt(m.getCenter());
	c.setScale(1, 1, 1);

	texture.loadData(p);
	texture.setTextureWrap(GL_REPEAT, GL_REPEAT);



	g1.setup(512, 64, ofVec2f(0.0f, 0.0f), ofVec2f(1.0f, 0), ofVec2f(0, 0.125f), ofVec2f(1.0f, 0.125f));
	g2.setup(512, 64, ofVec2f(0.0f, 0.125f), ofVec2f(1.0f, 0.125f), ofVec2f(0.0f, 0.25f), ofVec2f(1.0f, 0.25f));
	g3.setup(512, 64, ofVec2f(0.3f, 0.25f), ofVec2f(0.8f, 0.25f), ofVec2f(0.3f, 0.375f), ofVec2f(0.8f, 0.375f));

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	Kernel k(5);
	k.gaussianKernel(1.2f);


	ofPixels p2;
	ofPixels p3;
	p2.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
	p3.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);

	GreyProcessing::greyFromHS(p.getPixels(), p2.getPixels(), p.getWidth(), p.getHeight(), SATURATION);
	//GreyProcessing::greyFromLuminance(p.getPixels(), p2.getPixels(), p.getWidth(), p.getHeight(),BRIGHTNESS);
	GreyProcessing::greyStretch(p2.getPixels(), p2.getPixels(), p.getWidth(), p.getHeight(),200, 255);
	GreyProcessing::greyBlur(p2.getPixels(), p3.getPixels(), p.getWidth(), p.getHeight(), k, WRAP);
	//GreyProcessing::greyInvert(p3.getPixels(), p3.getPixels(), p.getWidth(), p.getHeight());

	Histogram h;
	h.update(p3.getPixels(), p.getWidth(), p.getHeight());


	ofSetColor(255);
	ofTexture t;
	t.setTextureWrap(GL_REPEAT, GL_REPEAT);
	t.loadData(p3);
	//t.draw(0, 0, 512, 512);

	ofFbo fbo1;
	ofFbo fbo2;
	ofFbo fbo3;
	fbo3.allocate(ofGetWindowWidth() - 512, ofGetWindowHeight(), GL_RGBA);
	
	GPUProcessing::blur(t, fbo1, k);

	GPUProcessing::normalmap(fbo1.getTextureReference(), fbo2, sobel_horizontal, sobel_vertical,1.0f);
	//GPUProcessing::invert(fbo2.getTextureReference(), fbo1, ofVec3f(1.0f,1.0f,0.0f) );
	
	//fbo2.draw(0, 512, 512, 512);

	fbo1.getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
	
	ofSetBackgroundColor(0);
	fbo3.begin();
	ofClear(0, 0, 0, 0);
	c.begin();

	m.draw(texture,fbo2.getTextureReference());

	c.end();
	fbo3.end();
	
	fbo3.draw(512,0,ofGetWindowWidth() - 512 , ofGetWindowHeight());
	

	ofSetBackgroundColor(0);

	g1.draw(0, 0, texture, fbo2.getTextureReference());
	g2.draw(0, 64, texture, fbo2.getTextureReference());
	g3.draw(0, 128,texture, fbo2.getTextureReference());

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
	int w = ofGetWindowWidth();
	int h = ofGetWindowHeight();

	m.setLightPos(ofVec4f( (x - w/2)/2,- (y - h/2)*-1/2, c.getPosition().z + objSize.z + 50,1.0f ) );
	g1.mesh.setLightPos(ofVec4f(x , y, 10, 1.0f));
	g2.mesh.setLightPos(ofVec4f(x, y, 10, 1.0f));
	g3.mesh.setLightPos(ofVec4f(x, y, 10, 1.0f));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
