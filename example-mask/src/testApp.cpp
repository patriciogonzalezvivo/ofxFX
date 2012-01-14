#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetWindowShape( 143, 290);
    
    mask.allocate(143, 290);
    
	topLayer.loadImage("topLayer.png");
	maskLayer.loadImage("mask.png");
	bottomLayer.loadImage("bottomLayer.png");
}

//--------------------------------------------------------------
void testApp::update(){
    ofSetColor(255,255);
    
    mask.beginSrc();
    topLayer.draw(0,0);
    mask.endSrc();
    
    mask.begin();
    ofClear(255);
    //ofSetColor(0,255);
    //ofCircle(mouseX, mouseY, 30);
    ofSetColor(255);
    maskLayer.draw(0 , mouseY - 15);
    mask.end();
    
    ofSetColor(255);
    mask.update();
        
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    bottomLayer.draw(0,0);
    
    mask.draw(0,0);
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