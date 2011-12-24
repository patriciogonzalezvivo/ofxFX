#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);

    ofEnableAlphaBlending();
	ofSetWindowShape(640, 480);

	image.loadImage("mem.gif");

    conway.allocate(640, 480);

}
//--------------------------------------------------------------
void testApp::update(){


    conway.begin();

    ofClear(255, 255, 255, 255);

    image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);

    ofSetColor(0);
    ofCircle(mouseX, mouseY, 5);
    conway.end();

    ofSetWindowTitle(ofToString(ofGetFrameRate()));

}

//--------------------------------------------------------------
void testApp::draw(){

    conway.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
