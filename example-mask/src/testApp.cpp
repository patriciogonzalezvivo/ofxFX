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
    
    mask.maskBegin();
    ofClear(255);
    
    //ofSetColor(0);
    //ofCircle(mouseX, mouseY, 30);
    
    ofSetColor(255);
    maskLayer.draw(0,mouseY - 15);
    
    mask.maskEnd();
        
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    bottomLayer.draw(0,0);
    
    mask.begin();
    topLayer.draw(0,0);
    mask.end();
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